#include <Arduino.h>
#include <SPI.h>
#include <../lib/Pixy.h>

void SetRandomLEDColor();
void LogPixyBlocks();
void WaitForSeconds(int seconds);

Pixy pixy;

void setup()
{
  Serial.begin(9600);
  Serial.print("Initiating...\n");

  pixy.init();
}

void loop()
{
  SetRandomLEDColor();
  LogPixyBlocks();
  WaitForSeconds(3);
}

void LogPixyBlocks()
{
   Serial.print(pixy.getBlocks());
}

void SetRandomLEDColor()
{
    //TODO log color code
    int r = random(0, 255);
    int g = random(0, 255);
    int b = random(0, 255);
    pixy.setLED(r,g,b);
}

void WaitForSeconds(int seconds)
{
  delay(seconds * 1000);
}
