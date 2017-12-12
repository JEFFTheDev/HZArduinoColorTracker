#include <Arduino.h>
#include <SPI.h>

#include <../lib/Pixy.h>
#include <SparkFunMiniMoto.h>

void setRandomLEDColor();
void logPixyBlocks();
void drive(Block b, int distance);
void stopMotors();
void rotate();
bool millisDelay(unsigned long millis);
void refresh();
int findBlockIndexFromSignature(int signature);

Pixy pixy;
unsigned long loopDelay = 25;
unsigned long stopAfter = 850;
unsigned long afterLastSign;
unsigned long afterLastRefresh;
unsigned long refreshAfter = 1000;

//Kleurcodes voor verscihllende omgevingen
int signatureBoven = 83;
int signatureBeneden = 10;
int currentSignature = 0;

Block lastBlock;
int maxAllowedOffset = 50;
int maxX = 319;
int maxY = 199;
int baseRotSpeed = 20;
int baseSpeed = 20;
int maxSpeed = 40;
int maxDistance = 200;

unsigned long time = millis();

MiniMoto motor0(0xC4); // A1 = 1, A0 = clear
MiniMoto motor1(0xC0); // A1 = 1, A0 = 1 (default)

void setup()
{
  currentSignature = signatureBoven;
  Serial.begin(9600);
  //Serial.print("Initiating...\n");
  pixy.init();
}

void loop()
{
  if (millisDelay(loopDelay))
  {
    refresh();
    int index = findBlockIndexFromSignature(currentSignature);

    if ( index > -1 )
    {
      lastBlock = pixy.blocks[index];
      drive(lastBlock, maxDistance);
      afterLastSign = millis();
      //When car hasn't seen the correct block for x milliseconds
    } else if (millis() > afterLastSign + stopAfter)
    {
      if (lastBlock.x < (maxX / 2) - maxAllowedOffset)
      {
        //Turn left
      }
      else
      {
        //Turn right
      }
      stopMotors();
      //rotate();
    }
  }
}

void refresh()
{
  if(millis() > afterLastRefresh + refreshAfter)
  {
    afterLastRefresh = millis();
    pixy.getBlocks();
  }
}

void rotate()
{
    motor0.drive(-15);
    motor1.drive(15);
}

void stopMotors()
{
  motor0.stop();
  motor1.stop();
}

int findBlockIndexFromSignature(int signature)
{
  for (int i = 0; i < pixy.blocks; i++)
  {
    if (pixy.blocks[i].signature == signature)
    {
      return i;
    }
  }
  return -1;
}

void drive(Block b, int distance)
{
  if (b.width < distance && b.width > 0)
  {
    int offset = map(b.width, 0, distance, 0, maxAllowedOffset);

    //Serial.println(offset);

    bool right = b.x > ((maxX / 2) + offset);
    bool left = b.x < ((maxX / 2) - offset);

    int x = b.x;
    x = left ? 319 - x : x;

    
    int speed = map(b.width, 0, distance, maxSpeed, 0);
    int rotationSpeed = map(x, 0, 319, 0, baseRotSpeed);

    int motor0Speed = right ? speed + rotationSpeed : speed;
    int motor1Speed = left ? speed + rotationSpeed : speed;

    //Serial.println(motor0Speed);

    motor0.drive(motor0Speed);
    motor1.drive(motor1Speed);

  }else
  {
    stopMotors();
  }
}

//seconds is too slow so it's waiting for some random integer
bool millisDelay(unsigned long milliSeconds)
{

  if (millis() > time + milliSeconds)
  {
    time = millis();
    //Serial.println("Seconds past...");

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
