#ifndef MOTOR_H_INCLUDED
#define MOTOR_H_INCLUDED

#include <Servo.h>
#include "ultra_sonic.h"
Servo motore1;  // Viene creato l'oggetto per il controllo del motore 1
Servo motore2;  // Viene creato l'oggetto per il controllo del motore 2
enum Dir { front, back ,left, right, v_stop };
void motor_initial () {
  motore1.attach(A8);  // left
  motore2.attach(A9);  // right wheel  
}
void stop_function()
{
        motore1.writeMicroseconds(1500); // left foreward
        motore2.writeMicroseconds(1500);  //right backward 
}

void avoid_collision();

void drive_verichel (char direct, char delay_time =0 )  //1800 around 1 cc 
{
  int init_time;
  int distance;
  switch ( direct) 
  {
    
    case front : 
        motore1.writeMicroseconds(1300); //left foreward
        motore2.writeMicroseconds(1300);  //right foreward 
          init_time = millis();
          if (delay_time > 0)
          {
            while((millis()-init_time) < 100*delay_time)
            {
              distance = get_distance();
              if(distance < 20)
              {
                Serial.print("\n\nEntering avoidCollision function with distance = ");
                Serial.println(distance);
                avoid_collision();
                break;
              }
            }
          }
        break ;
    case back : 
        motore1.writeMicroseconds(2000); //left backward
        motore2.writeMicroseconds(2000);  //right backward 
         if (delay_time !=0 )
        {
          delay(100*delay_time); // distance
          stop_function();
        }
        break ;
    case left : 
        motore1.writeMicroseconds(1100); //left foreward
        motore2.writeMicroseconds(1900);  //right backward 
         if (delay_time !=0 )
        {
          delay(100*delay_time); // distance
          stop_function();
        }
        break;
    case right : 
        motore1.writeMicroseconds(1900); //left backward
        motore2.writeMicroseconds(1100); //right foreward, bias due to weak motor
         if (delay_time !=0 )
        {
        delay(100*delay_time); // distance
        stop_function();
        }
        break;
    case v_stop: 
       stop_function();
        break;
   }
}

void avoid_collision()
{
   stop_function();

   delay(100);

   drive_verichel(left,10);
   double left_distance = 0;
   left_distance = get_distance();
   delay(500);
   
   drive_verichel(right,10);
   delay(100);
   drive_verichel(right,10);
   double right_distance = 0;
   right_distance = get_distance();
   delay(500);

   if(left_distance>right_distance)
   {
     drive_verichel(left,10);
     delay(100);
     drive_verichel(left,10);
     delay(100);
     drive_verichel(front,20);
   }
   else
   {
     drive_verichel(front,20);
   }
   
 }

#endif //MOTOR_H_INCLUDED


