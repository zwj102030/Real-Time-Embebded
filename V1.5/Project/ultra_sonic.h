#include "arduinoFFT.h"
//#include "Motion.h"
#include <Servo.h>
#include <Ping.h>

#define trigPin 21
#define echoPin 20

extern double get_distance()  
    {
      long duration, distance;
      digitalWrite(trigPin, LOW);  
      delayMicroseconds(2); 
      digitalWrite(trigPin, HIGH);
    
      delayMicroseconds(10); 
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance = (duration/2) / 29.1;
      return distance;   //retunr value in cm 
    
    }


void ultra_sonic_setup()   // setting up the ultra_sonic sensor 
{      
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}


