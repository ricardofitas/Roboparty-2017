/* 
 This example was created by José Cruz (www.botnroll.com)
 on 28 November 2013
 
 Modified by José Fernandes on 21 July 2014
 Revised by José Cruz on 23 March 2016 
 
 This code example is in the public domain. 
 http://www.botnroll.com

This program detects automatic start on the race.
*/

#include <BnrOneA.h>   // Bot'n Roll ONE A library
#include <SPI.h>       // SPI communication library required by BnrOne.cpp
#include <Wire.h>
BnrOneA one;           // declaration of object variable to control the Bot'n Roll ONE A

//constants definition
#define SSPIN  2       // Slave Select (SS) pin for SPI communication
#define smin 300 //Line follower limit between white and black 
#define ADDRESS 0x60  
#define VEL_NORMAL 70
float batmin=9.5;  //13.5 Lithium

int vel=VEL_NORMAL;


void setup()
{
  Wire.begin();                                               // Conects I2C
  Serial.begin(57600);
	one.spiConnect(SSPIN);  // start SPI communication module
	one.stop();             // stop motors
	one.lcd1("IR testing"); // print on LCD line 1
	one.lcd2("STOP");       // print on LCD line 2
	one.obstacleEmitters(OFF); //Deactivate obstacles IR emitters
	delay(4000);           //Time to stabilize IR sensors (DO NOT REMOVE!!!)
        static byte start=0;
 	while(!start)
        {
          start=automatic_start();
        }
 	one.move (vel,vel); //The robot starts by moving forwards
 	one.lcd2("GO"); //Remove when racing for best performance!
}


int read_pitch()
{
char pitch;                // Store pitch values of CMPS10, chars are used because they support signed value

   Wire.beginTransmission(ADDRESS);           //start communication with CMPS10
   Wire.write(4);                             //Send the register we wish to start reading from
   Wire.endTransmission();

   Wire.requestFrom(ADDRESS, 1);              // Request 4 bytes from CMPS10
   if(Wire.available() == 1);               // Wait for bytes to become available
   pitch = Wire.read();

return (int)pitch;
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


void loop()
{
int sensor0 = one.readAdc(0);
      int sensor1 = one.readAdc(1);
      int sensor2 = one.readAdc(2);
      int sensor3 = one.readAdc(3);
      int sensor4 = one.readAdc(4);
      int sensor5 = one.readAdc(5);
      int sensor6 = one.readAdc(6);
      int sensor7 = one.readAdc(7);

      int pitch;
      pitch=read_pitch();
      if(pitch>2) vel=85;
      else vel=VEL_NORMAL;
      if(sensor0 > smin && sensor1 > smin && sensor2 < smin && sensor3 < smin && sensor4 < smin && sensor5 < smin && sensor6 < smin && sensor7 < smin)//11000000
      {
        one.move(-10,30);
      }
      else if(sensor0 < smin && sensor1 > smin && sensor2 > smin && sensor3 < smin && sensor4 < smin && sensor5 < smin && sensor6 < smin && sensor7 < smin)//01100000
      {
        one.move(15,vel);
      }
      else if(sensor0 < smin && sensor1 < smin && sensor2 > smin && sensor3 > smin && sensor4 < smin && sensor5 < smin && sensor6 < smin && sensor7 < smin)//00110000
      {
        one.move(7.5,vel);
      }
      else if(sensor0 < smin && sensor1 < smin && sensor2 < smin && sensor3 > smin && sensor4 > smin && sensor5 < smin && sensor6 < smin && sensor7 < smin)//00011000
      {
        one.move(vel,vel);
      }
      else if(sensor0 < smin && sensor1 < smin && sensor2 < smin && sensor3 < smin && sensor4 > smin && sensor5 > smin && sensor6 < smin && sensor7 < smin)//00001100
      {
        one.move(vel,7.5);
      }
       else if(sensor0 < smin && sensor1 < smin && sensor2 < smin && sensor3 < smin && sensor4 < smin && sensor5 > smin && sensor6 > smin && sensor7 < smin)//00000110
      {
        one.move(vel,15);
      }
      else if(sensor0 < smin && sensor1 < smin && sensor2 < smin && sensor3 < smin && sensor4 < smin && sensor5 < smin && sensor6 > smin && sensor7 > smin)//00000011
      {
        one.move(30,-10);
      }  
}
