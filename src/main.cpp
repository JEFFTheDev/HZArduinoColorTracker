#include <Arduino.h>
#include <SPI.h>

#include <../lib/Pixy.h>
#include <SparkFunMiniMoto.h>

void setRandomLEDColor();
void logPixyBlocks();
void drive(Block b);
bool millisDelay(unsigned long millis);
int findBlockIndexFromSignature(int signature);

Pixy pixy;
unsigned long loopDelay = 250;
int maxAllowedOffset = 38;
int maxX = 319;
int maxY = 199;

//123 = 83 because of decimal stuff??
int decimalSignature = 83;
unsigned long time = millis();

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
  if(millisDelay(loopDelay))
  {
    
   int index = findBlockIndexFromSignature(decimalSignature);

   if(index > -1 && pixy.blocks[index].signature == decimalSignature)
   {
     drive(pixy.blocks[index]);
   }
   else
    {
      motor0.stop();
      motor1.stop();
    }
  }
}

// Signature 12 in Pixymon translates to 10 decimal (pixy.blocks)
void logPixyBlocks()
{
   for(int i = 0; i < pixy.getBlocks(); i++)
   {
      pixy.blocks[i].print();
   }
}


int findBlockIndexFromSignature(int signature)
{
   for (int i=0; i< pixy.getBlocks(); i++)
   {
      if(pixy.blocks[i].signature == signature)
      {
         return i;
      }
   }
   return -1;
}

void drive(Block b)
{
   bool left = b.x > ((maxX / 2) + maxAllowedOffset);
   bool right = b.x < ((maxX / 2) - maxAllowedOffset);

   int x = b.x;
   x = right ? 319 - x : x;

   int speed = map(x,0,319,0,15);

   Serial.println(speed);

   int motor0Speed = left ? 18 + speed : 20;
   int motor1Speed = right ? 18 + speed : 20;

   motor0.drive(motor0Speed);
   motor1.drive(motor1Speed);
}

//seconds is too slow so it's waiting for some random integer
bool millisDelay(unsigned long milliSeconds)
{

   if(millis() > time +  milliSeconds)
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
