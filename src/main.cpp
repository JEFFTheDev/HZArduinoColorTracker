#include <Arduino.h>
#include <SPI.h>

#include <../lib/Pixy.h>
#include <SparkFunMiniMoto.h>

void setRandomLEDColor();
void logPixyBlocks();
void waitForSeconds(int seconds);
void drive();
int getIteratorBlock(int x);
int calculateAngle(int block);
int getOffset(Block left, Block right);

Pixy pixy;
int loopDelay = 1;
int maxAllowedOffset = 15;
int maxX = 319;
int maxY = 199;

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

// Signature 12 in Pixymon translates to 10 decimal (pixy.blocks)
void logPixyBlocks()
{
   uint16_t blocks = pixy.getBlocks();

   if(blocks > 0)
   {
    // lekker hardcoded
      if(pixy.blocks[0].signature == 10)
      {
        //post its found, turn on
//        Serial.print("found signature 12 en eigenlijk dus 10\n");
        // Serial.print("x = ");
        pixy.blocks[0].print();
        // Serial.println("in beeld");
        // Serial.print("Angle = ");
        //motor0.drive(25);
        //motor1.drive(25);
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
  //Drive left
  if(pixy.blocks[0].x > ((maxX / 2) + maxAllowedOffset))
  {
      int speed = calculateAngle(getIteratorBlock(pixy.blocks[0].x));
      motor0.drive(15 + speed);
      motor1.drive(10);
  }
  //Drive right
  else if(pixy.blocks[0].x < ((maxX / 2) - maxAllowedOffset))
  {
       int speed = calculateAngle(getIteratorBlock(pixy.blocks[0].x));
       motor1.drive(15 + speed);
       motor0.drive(10);
  }
  else
  {
    motor0.drive(15);
    motor1.drive(15);
  }
}

//seconds is too slow so it's waiting for some random integer
void waitForSeconds(int seconds)
{
  delay(seconds * 100);
}

int getIteratorBlock(int x)
{
   int block = x / (x/11);


   return block;
}

int calculateAngle(int block)
{
  switch (block) {
    case 1:
     return 15;
     break;
    case 2:
     return 12;
     break;
    case 3:
     return 9;
     break;
    case 4:
     return 6;
     break;
    case 5:
     return 3;
     break;
    case 7:
      return 3;
      break;
    case 8:
      return 6;
      break;
    case 9:
      return 9;
      break;
    case 10:
      return 12;
      break;
    case 11:
      return 15;
      break;
    default:
      return 1;
      break;
  }

}



//motors kunnen versneld worden, dit moet aan de hand van de y waarde van de pixy
//motors kunnen draaien, dit moet aan de hand van de x waarde van de pixy

//x is van 0 tot 319, het midden van de camera is dus 159,5
//y is van 0 tot 199, het midden van de camera is dus 99,5

//motor mag van -100 tot 100 voor snelheid

//http://cmucam.org/projects/cmucam5/wiki/Arduino_API

//arduino map
