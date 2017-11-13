#include <Arduino.h>
#include <SPI.h>

#include <../lib/Pixy.h>
#include <SparkFunMiniMoto.h>

void setRandomLEDColor();
void logPixyBlocks();
void drive();
bool millisDelay(unsigned long millis);

Pixy pixy;
unsigned long loopDelay = 250;
int maxAllowedOffset = 38;
int maxX = 319;+
int maxY = 199;
unsigned long time;

MiniMoto motor0(0xC4); // A1 = 1, A0 = clear
MiniMoto motor1(0xC0); // A1 = 1, A0 = 1 (default)

void setup()
{
  Serial.begin(9600);
  Serial.print("Initiating...\n");
  pixy.init();
}

void loop()
{
  //setRandomLEDColor();
  if(millisDelay(loopDelay))
  {
    logPixyBlocks();
  }
  //delay(1000);
}

void setRandomLEDColor()
{
    int r = random(0, 255);
    int g = random(0, 255);
    int b = random(0, 255);
    pixy.setLED(r,g,b);
}

// Signature 12 in Pixymon translates to 10 decimal (pixy.blocks)
void logPixyBlocks()
{
   uint16_t blocks = pixy.getBlocks();

   if(blocks > 0)
   {
      if(pixy.blocks[0].signature == 10)
      {
        pixy.blocks[0].print();
        //drive();
        drive();
      }
     }
   else
      {
        motor0.stop();
        motor1.stop();
      }
}

void drive()
{
   bool left = pixy.blocks[0].x > ((maxX / 2) + maxAllowedOffset);
   bool right = pixy.blocks[0].x < ((maxX / 2) - maxAllowedOffset);

   int x = pixy.blocks[0].x;
   x = right ? 319 - x : x;

   int speed = map(x,0,319,0,15);

   Serial.println(speed);

   int motor0Speed = left ? 25 + speed : 20;
   int motor1Speed = right ? 25 + speed : 20;

   motor0.drive(motor0Speed);
   motor1.drive(motor1Speed);
}

//seconds is too slow so it's waiting for some random integer
bool millisDelay(unsigned long milliSeconds)
{
   unsigned long millisArduino = millis();
   if(millisArduino > time +  milliSeconds)
   {
     time = millis();
     Serial.println("Seconds past...");

     return true;
   }

   return false;
}



//motors kunnen versneld worden, dit moet aan de hand van de y waarde van de pixy
//motors kunnen draaien, dit moet aan de hand van de x waarde van de pixy

//x is van 0 tot 319, het midden van de camera is dus 159,5
//y is van 0 tot 199, het midden van de camera is dus 99,5

//motor mag van -100 tot 100 voor snelheid

//http://cmucam.org/projects/cmucam5/wiki/Arduino_API

//arduino map
