#include <Servo.h>
#include "motor.h"
#include "FFT.h"
#include "ultra_sonic.h"
//SAMPLES 128            Must be a power of 2
//SAMPLING_FREQUENCY 25600 Hz, must be less than 80000 due to ADC
#define Volume_Thershold 10//if the Volume is lager than this, it will go
#define distance_Thershold 40//if the distance is lager than this, it will go
#define frequency_diff 200//the max variation of the frequncy
//int frequency_hz[10]={10000,9000,8000,7000,6000,5000,4000,3000,2000,1000};//the frequency of the ten mines
int distance;
int Locate_Freq (double current_frequency);

double Freq_Value_Last;
double Freq_Value_Current;
void print_mic_info ();


void setup()
{
  FFT_init() ;
  motor_initial () ;
  ultra_sonic_setup();
  Serial.begin(115200);
}

void loop()
{   char index_current;
    char index_last;
    int current_Freq;
    int target_Freq; 
    //distance =get_distance();
   index_current =  Sampling();
   char rotate_check_flag =1 ;
   while ( rotate_check_flag )
   {
    drive_verichel(left,5);
    index_current = Sampling();
   if (index_current > index_last ) //find the larrget freqncy first
    {
    index_last =index_current;
    Freq_Value_Last =totReadings[index_last] ;
    }
    else if (index_current ==index_last) 
    {
        if (  totReadings[index_last]>Freq_Value_Last) 
        {
          Freq_Value_Last =totReadings[index_last];
          rotate_check_flag=1;
        }
        else 
        {
          drive_verichel(right,5);
          rotate_check_flag=0;
        }
    }
     
   }
   if (get_distance() >15)
      {
      drive_verichel(front,20);      
      }
    else 
    {
    stop_function();
    }
    
 }



void print_info ()
{  Serial.print("Distance "); Serial.print(distance); Serial.println(" cm");
  for(size_t i = 0; i < 10; i++)
    {
      Serial.print(5000 + (500 * i)); Serial.print(" Amplitude: "); 
      Serial.println(totReadings[i]);
    }
    Serial.print("\n\n");
    Serial.print("This took "); Serial.print(millis() - startTime); Serial.println(" milliseconds");
}

void find_mine_dircetion (){  
 
}































//
//int Locate_Freq (double current_frequency)
//{ 
//  int Freq_index;
//    for(int i=10;i>=1;i--)
//  {
//    if(abs(current_frequency-frequency_hz[i])<frequency_diff)
//    {
//      Freq_index =i;  
//      break;
//    }
//  }
//  return frequency_hz[Freq_index]; // return target Freq
//}













