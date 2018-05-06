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
{    char index;
    int current_Freq;
    int target_Freq; 
    //distance =get_distance();
    Sampling();
    
    
   while ( rotate_check_flag )
   {
    drive_verichel(left,1);
   index = Sampling();
    if (  totReadings[index] >Freq_Value_Last) 
    {
      Freq_Value_Last =totReadings[index];
      rotate_check_flag=0;
    }
    else 
    {
      stop_function();
     
    }
     
   }
   rotate_check_flag=1;
   if (get_distance() >15)
      {
      drive_verichel(front);      
      }
    else 
    {
    stop_function();
    drive_verichel(left,5);
    drive_verichel(left,5);
    }
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



