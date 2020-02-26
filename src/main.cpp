/************************************************************
 * 		File   : main.cpp
 *  	Date   : 02.09.2020
 *  	Organization:
 *      Authors : Barry Lim
 *************************************************************/

#include "Arduino.h"
#include "stdio.h"

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

#define ONE_WIRE_BUS 2

OneWire *oneWire = NULL;
DallasTemperature *temperatureSensor = NULL;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup()
{
	Serial.begin(28800);
	Serial.println("Digital Thermometer init ... ");

	oneWire = new OneWire();
	oneWire->setPin(ONE_WIRE_BUS);
	temperatureSensor = new DallasTemperature();

	temperatureSensor->setOneWire(oneWire);
	temperatureSensor->begin();

	// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
	if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
		Serial.println(F("SSD1306 allocation failed"));
		for(;;); // Don't proceed, loop forever
	}

	// configure display text settings
	display.setTextSize(1);
	display.setTextColor(SSD1306_WHITE); // Draw white text
	display.setCursor(0, 0);     // Start at top-left corner
	display.cp437(true);         // Use full 256 char 'Code Page 437' font

	// Show initial display buffer contents on the screen --
	// the library initializes this with an Adafruit splash screen.
	display.display();
	delay(1000); // Pause for 1 seconds

	// Clear the buffer
	display.clearDisplay();
}


// function to display temperature on lcd
void displayTemperature(float tempDegC){
	char tens = ((char)tempDegC / 10) + '0';
	char ones = ((char)tempDegC % 10) + '0';
	int decimal1;
	int decimal2;

	tempDegC *= 100;
	decimal1 = (char)((int)tempDegC / 10 % 10) + '0';
	decimal2 = (char)((int)tempDegC % 10) + '0';

	display.clearDisplay();
	display.setCursor(0, 10);

	display.setTextSize(3);
	display.write(tens);
	display.write(ones);
	display.write('.');
	display.write(decimal1);
	display.write(decimal2);	// 2 decimal place resolution


	display.setTextSize(1);
	display.write(' ');
	display.write('o');
	display.setTextSize(3);
	display.write('C');

	display.display();
	delay(500);
}


// function to print the temperature for a device
void printTemperature(void)
{
	float tempDegC = 0.0;
	tempDegC = temperatureSensor->getTempCByIndex(0);
	Serial.print(tempDegC);
	Serial.println(" Deg Celcius");

//	Serial.print(DallasTemperature::toFahrenheit(tempDegC)); // Converts tempDegC to Fahrenheit
//	Serial.println("  Deg Fahrenheit");

	displayTemperature( tempDegC );
}


void loop()
{
	  // call sensors.requestTemperatures() to issue a global temperature
	  // request to all devices on the bus
	  temperatureSensor->requestTemperatures();
	  printTemperature();
}
