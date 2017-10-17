#include <Arduino.h>
#include <SPI.h>
#include <../lib/Pixy.h>

void setRandomLEDColor();
void logPixyBlocks();
void waitForSeconds(int seconds);
int getOffset(Block left, Block right);

Pixy pixy;
int loopDelay = 1;
int maxAllowedOffset = 5;

void setup()
{
  Serial.begin(9600);
  Serial.print("Initiating...\n");

  pixy.init();
}

void loop()
{
  setRandomLEDColor();
  logPixyBlocks();
  waitForSeconds(loopDelay);
}

void setRandomLEDColor()
{
    int r = random(0, 255);
    int g = random(0, 255);
    int b = random(0, 255);
    pixy.setLED(r,g,b);
}

void logPixyBlocks()
{
   uint16_t blocks = pixy.getBlocks();

   Serial.print(blocks);

   if(blocks > 0)
   {
     for(int i = 0; i < blocks; i++)
     {
       pixy.blocks[i].print();

       if(blocks == 2 && pixy.blocks[0].signature != pixy.blocks[1].signature)
       {
         Serial.print("Found 2 signatures");
       }
       else if(blocks == 1)
       {
         if(pixy.blocks[0].signature == 1)
         {
           Serial.print("Green...");
         }
         else
         {
           Serial.print("Pink...");
         }
       }
     }
   }
}

void waitForSeconds(int seconds)
{
  delay(seconds * 1000);
}
