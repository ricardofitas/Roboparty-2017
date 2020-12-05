/* 
 This example was created by José Cruz (www.botnroll.com)
 on 28 November 2013
 
 This code example is in the public domain. 
 http://www.botnroll.com
 
 Revised by Ricardo Fitas on the 3rd March 2017
 
*/
 
#include <SPI.h>                   // required by BnrOne.cpp
#include <BnrOneA.h>

//constants definitions
#define SSPIN  2    //Slave Select (SS) pin for SPI communication
#define M1  1       //Motor1
#define M2  2       //Motor2
#define smin 300    //521

#define KP 0.53 //0.2 ou 0.25
#define KD 10.10 //0.92 ou 0.94
#define M1_DEFAULT_SPEED 50 //30
#define M2_DEFAULT_SPEED 50 //30
#define M1_MAX_SPEED 70  //74
#define M2_MAX_SPEED 70 //74
#define MIDDLE_SENSOR 4//3 ja foi 5
#define NUM_SENSORS  8      // number of sensors used
#define TIMEOUT     2500
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2
#define DEBUG 0 // set to 1 if serial debug output needed

BnrOneA one;         //declaration of object variable to control the Bot'n Roll ONE

//Variables to read from Bot'n Roll ONE
byte firm_int=0;      // the firmware version of your Bot'n Roll ONE board (visit http://www.botnroll.com/one/ to check if your version is updated)
byte firm_dec=0;      // the firmware version of your Bot'n Roll ONE board (visit http://www.botnroll.com/one/ to check if your version is updated)
byte firm_dev=0;      // the firmware version of your Bot'n Roll ONE board (visit http://www.botnroll.com/one/ to check if your version is updated)
float battery=0;       // battery reading
int lastError=0;

void setup()
{
  one.spiConnect(SSPIN);  // start SPI communication module
  one.stop();             // stop motors
  //one.lcd1("IR testing"); // print on LCD line 1
  //one.lcd2("STOP");       // print on LCD line 2
  one.obstacleEmitters(OFF); //Deactivate obstacles IR emitters
  delay(4000);           //Time to stabilize IR sensors (DO NOT REMOVE!!!)
        static byte start=0;
  while(!start)
        {
          start=automatic_start();
        }
  one.move(50,50); //The robot starts by moving forwards
 // one.lcd2("GO"); //Remove when racing for best performance!

}

bool automatic_start()
{ 
  bool actstate=one.readIRSensors(); //read actual IR sensors state
  
  if(!actstate) //If state is LOW
  {
    unsigned long int tempo_A=millis(); //read time
    while(!actstate) //while state is LOW
    {
      actstate=one.readIRSensors(); //read actual IR sensors state
      if((millis()-tempo_A)>50) //if time is low for more than 50ms
      {
        return true; //Start Race
      }
    }
  }
  return false;
}


void loop() {
  
  //Read the 8 line sensors  
    int sensor0 = one.readAdc(0);
    int sensor1 = one.readAdc(1);
    int sensor2 = one.readAdc(2);
    int sensor3 = one.readAdc(3);
    int sensor4 = one.readAdc(4);
    int sensor5 = one.readAdc(5);
    int sensor6 = one.readAdc(6);
    int sensor7 = one.readAdc(7);
    
    int somatotal = 0;
    int qtdvalores = 0;
    int leituralinha = 0;
    
    float error;
    int MotorSpeed;
    int leftMotorSpeed;
    int rightMotorSpeed;
    int motorSpeed;
    
    if (sensor0 > smin)
    {
      qtdvalores = qtdvalores;
    }
    if (sensor1 > smin)
    {
      somatotal = somatotal + 1000;
      qtdvalores = qtdvalores + 1;
    }
    if (sensor2 > smin)
    {
      somatotal = somatotal + 2000;
      qtdvalores = qtdvalores + 1;
    }
    if (sensor3 > smin)
    {
      somatotal = somatotal + 3000;
      qtdvalores = qtdvalores + 1;
    }
    if (sensor4 > smin)
    {
      somatotal = somatotal + 4000;
      qtdvalores = qtdvalores + 1;
    }
    if (sensor5 > smin)
    {
      somatotal = somatotal + 5000;
      qtdvalores = qtdvalores + 1;
    }
    if (sensor6 > smin)
    {
      somatotal = somatotal + 6000;
      qtdvalores = qtdvalores + 1;
    }
    if (sensor7 > smin)
    {
      somatotal = somatotal + 7000;
      qtdvalores = qtdvalores + 1;
    }
    leituralinha = somatotal/qtdvalores;
    error=leituralinha-3500;

    motorSpeed = KP * error + KD * (error - lastError);
    lastError = error;

    leftMotorSpeed = M1_DEFAULT_SPEED + motorSpeed;
    rightMotorSpeed = M2_DEFAULT_SPEED - motorSpeed;    
    
    if (leftMotorSpeed > M1_MAX_SPEED ) leftMotorSpeed = M1_MAX_SPEED; // limit top speed
    if (rightMotorSpeed > M2_MAX_SPEED ) rightMotorSpeed = M2_MAX_SPEED; // limit top speed
    if (leftMotorSpeed < 0) leftMotorSpeed = 0; // keep motor above 0
    if (rightMotorSpeed < 0) rightMotorSpeed = 0;
    
    one.move(leftMotorSpeed,rightMotorSpeed);
    delay (10);
}



