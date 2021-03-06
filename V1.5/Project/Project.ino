#include <Servo.h>
#include "motor.h"
#include "FFT.h"
#include "ultra_sonic.h"
//SAMPLES 128            Must be a power of 2
//SAMPLING_FREQUENCY 25600 Hz, must be less than 80000 due to ADC
#define Volume_Threshold 50//if the Volume is larger than this, it will go
#define distance_Threshold 40//if the distance is lager than this, it will go
#define frequency_diff 200//the max variation of the frequncy



//gobal varible
int distance; //distance, returned from the ultra_sonic 
//int Locate_Freq (double current_frequency);
double Freq_Value_Last;
double Freq_Value_Current;
void print_info ();       //print the all info though serial port 
double Mag[3] = {0,0,0};  //Mag array will contain freq. volume from left [0], middle [1] , and right Mag [2]
int index_i = 0;          //frequency increment counter 
bool max_flag =0;         //the current freq also has the max magnitude
int counter_i =0;

void print_info ()   //print the all info though serial port 
{
    distance =get_distance();   //get the distance from ultra_sonic sensor. 
    Serial.print("Distance "); Serial.print(distance); Serial.println(" cm");
    for(size_t i = 0; i < 10; i++)
    {
      Serial.print(5000 + (500 * i)); Serial.print(" Amplitude: "); 
      Serial.println(totReadings[i]);
    }
     
    Serial.print("\n index:  ");
    Serial.print(index_i*500+5000);    //print the current frequency 
    Serial.print("\n Mag[0]:  ");
    Serial.print(Mag[0]);              //left magnitude 
    Serial.print(" Mag[1]:  ");
    Serial.print(Mag[1]);              //middle magnitude 
    Serial.print(" Mag[2]:  ");
    Serial.print(Mag[2]);              //right magnitude 
    Serial.print("\n ");
    Serial.print("This took "); Serial.print(millis() - startTime); Serial.println(" milliseconds");  //total time to do the FFT smapling 
    
}

void initial_array()  // first step to fill up left-middle-right  array .
{
  Mag[0]=0;
  Mag[1]=0;
  Mag[2]=0;
  check_index(); //Check if frequency needs to be switched
  Sampling();
  Mag[1] =totReadings[index_i];
  drive_verichel(right,5);
  delay(50);
  Sampling();
  Mag[0] = totReadings[index_i];
  drive_verichel(left,5);
  delay(100);
  drive_verichel(left,5);
  delay(50);
  Sampling();
  Mag[2] = totReadings[index_i];
}



void  check_direction(bool goingLeft)
{
 check_index();  
 print_info ();
      if ((Mag[1]-Mag [0]>=Volume_Threshold) && (Mag[1]-Mag [2]>=Volume_Threshold) )  //check if middle magnitude is greater than both left and right  
      {
        if(goingLeft)
        {
          drive_verichel(right, 5);
        }
        else
        {
          drive_verichel(left, 5);
        }
        delay(100);
        drive_verichel (front,30);
        delay(500);
      }
      else if (Mag[0]-Mag [1]>= 0)
      {
        if(!goingLeft)
        {
          drive_verichel(left, 5);
          delay(100);
        }
        drive_verichel (left,5);
        delay(50);
        Sampling();
        Mag[2]=Mag[1];
        Mag[1]=Mag[0];
        Mag[0] = totReadings[index_i];
        check_direction(true);
      }
      
    else if (Mag[2]-Mag [1] >= 0)
    {
        if(goingLeft)
        {
          drive_verichel(right, 5);
          delay(100);
        }
        drive_verichel (right,5);
        Sampling();
        Mag[0]=Mag[1];
        Mag[1]=Mag[2];
        Mag[2] = totReadings[index_i];
        check_direction(false);
    }
    else
    {
        drive_verichel(left, 5);
        delay(500);
        drive_verichel(left, 5);
        delay(100);
        initial_array();
        check_direction(true);
    }
}

void finish_blink_red() //Call when the final frequency has been reached
{
  for(;;)
  {
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
  }
}

void check_index()
{
    for (int i= index_i; i<10;i++)
    {
     if ( totReadings[i]<totReadings [index_i])
        counter_i++;
    }
   if (max_flag==(10-counter_i))  //the current has maximum value 
        max_flag =1;
   if (get_distance() <=15 &&max_flag ==1)
   {
       if(index_i == 9)
       {
        finish_blink_red();
       }
       index_i++;
       max_flag=0;
   }
}

void setup()
{
  delay(5000);
  FFT_init() ;
  motor_initial () ;
  ultra_sonic_setup();
  Serial.begin(115200);
}

void loop()
{   
  initial_array();  //Populate 
  delay(100); 
  check_direction(true); // do the first check  
}

