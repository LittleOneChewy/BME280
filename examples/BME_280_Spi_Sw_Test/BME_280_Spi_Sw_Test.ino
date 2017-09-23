/*
BME280 Spi Sw Test.ino

This code shows how to record data from the BME280 environmental sensor
using Spi interface. This file is an example file, part of the Arduino
BME280 library.

GNU General Public License

Written: Dec 30 2015.
Last Updated: Sep 23 2017.

Connecting the BME280 Sensor:
Sensor              ->  Board
-----------------------------
Vin (Voltage In)    ->  3.3V
Gnd (Ground)        ->  Gnd
SDA (Serial Data)   ->  A4 on Uno/Pro-Mini, 20 on Mega2560/Due, 2 Leonardo/Pro-Micro
SCK (Serial Clock)  ->  A5 on Uno/Pro-Mini, 21 on Mega2560/Due, 3 Leonardo/Pro-Micro

 */

#include <BME280SpiSw.h>

#define SERIAL_BAUD 115200

#define CHIP_SELECT_PIN 10
#define MOSI_PIN 11
#define MISO_PIN 12
#define SCK_PIN  13


BME280SpiSw bme(CHIP_SELECT_PIN, MOSI_PIN, MISO_PIN, SCK_PIN);

bool metric = false;

//////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(SERIAL_BAUD);

  while(!Serial) {} // Wait

  while(!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }
}

//////////////////////////////////////////////////////////////////
void loop()
{
   printBME280Data(&Serial);
   printBME280CalculatedData(&Serial);
   delay(1000);
}

//////////////////////////////////////////////////////////////////
void printBME280Data
(
   Stream* client
)
{
   float temp(NAN), hum(NAN), pres(NAN);
   uint8_t pressureUnit(3);   // unit: B000 = Pa, B001 = hPa, B010 = Hg, B011 = atm, B100 = bar, B101 = torr, B110 = N/m^2, B111 = psi
 
   bme.read(pres, temp, hum, metric, pressureUnit);   // Parameters: (float& pressure, float& temp, float& humidity, bool celsius = false, uint8_t pressureUnit = 0x0)

   client->print("Temp: ");
   client->print(temp);
   client->print("°"+ String(metric ? 'C' :'F'));
   client->print("\t\tHumidity: ");
   client->print(hum);
   client->print("% RH");
   client->print("\t\tPressure: ");
   client->print(pres);
   client->print(" atm");
}

//////////////////////////////////////////////////////////////////
void printBME280CalculatedData
(
   Stream* client
)
{
   float altitude = bme.alt(metric);
   float dewPoint = bme.dew(metric);

   client->print("\t\tAltitude: ");
   client->print(altitude);
   client->print((metric ? "m" : "ft"));
   client->print("\t\tDew point: ");
   client->print(dewPoint);
   client->println("°"+ String(metric ? 'C' :'F'));
}
