/* 
 This example was created by José Cruz (www.botnroll.com)
 on the 29th January 2014
 
 This code example is in the public domain. 
 http://www.botnroll.com

Description:

*/
#include <BnrOneA.h>   // Bot'n Roll ONE A library
#include <SPI.h>      // SPI communication library required by BnrOne.cpp
BnrOneA one;           // declaration of object variable to control the Bot'n Roll ONE A

//constants definition
#define SSPIN  2      // Slave Select (SS) pin for SPI communication
int evitavolta=0;   // Para evitar que volte para trás

void setup() 
{ 
    //setup routines -> runs only once when program starts
    Serial.begin(57600);     // set baud rate to 57600bps for printing values at serial monitor. Press (ctrl+shift+m) after uploading    
    one.spiConnect(SSPIN);  // start the SPI communication module
    one.stop();              // stop motors
    delay(5);                // wait 5 milieconds
    one.obstacleEmitters(ON);// activate obstacles detection
    delay(2000);             // wait 5 miliseconds
    one.move(70,71);     // Move Forward
    
}

void loop()
{
    byte obstacles=one.obstacleSensors(); //Read obstacle sensors
    switch(obstacles)
    {
        case 0:   // no obstacles detected
            //one.lcd2("  No Obstacles");
            evitavolta = 0;
            one.move(85,86);     // Move Forward
            break;
        case 1:   // obstacle detected on Left sensor
            //one.lcd2("   Left Sensor");
            evitavolta = 1;
            one.move(90,85);
            one.move(75,60);
            one.move(65,40);
            one.move(45,0);    // Rotate Right
            one.move(40,-10);
            break;
        case 2:   // obstacle detected on Right sensor
            //one.lcd2("  Right Sensor");
            evitavolta = 2;  
            one.move(85,90);
            one.move(60,75);
            one.move(40,55);
            one.move(0,45);    // Rotate Left
            one.move(-10,40);
            break;
        case 3:   // obstacle detected on both sensors
            //one.lcd2("  Both Sensors");
            //one.stop();
            if (evitavolta == 1)
            {
            one.move(90,85);
            one.move(75,60);
            one.move(65,40);
            one.move(45,0);    // Rotate Right
            one.move(40,-10);
            }
            else if (evitavolta == 2)
            {
            one.move(85,90);
            one.move(60,75);
            one.move(40,55);
            one.move(0,45);    // Rotate Left
            one.move(-10,40);
            }
            else
            {
              one.move (-10,40);
            }
            delay(200);     
            break;
    }
    // delay(10);
}

