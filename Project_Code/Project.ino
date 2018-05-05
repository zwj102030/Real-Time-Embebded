#include <Servo.h>
#include "motor.h"
#include "FFT.h"

#define SAMPLES 128             //Must be a power of 2
#define SAMPLING_FREQUENCY 25600//Hz, must be less than 80000 due to ADC

#define Volume_Thershold 10;//if the Volume is lager than this, it will go
#define distance_Thershold 40;//if the distance is lager than this, it will go
#define frequency_diff 200;//the max variation of the frequncy
int frequency_hz[10]={10000,9000,8000,7000,6000,5000,4000,3000,2000,1000};//the frequency of the ten mines

int Locate_Freq (double current_frequency);


void setup()
{
  FFT_init() ;
  motor_initial () 
}




void loop()
{
int current_Freq;
int target_Freq; 
current_Freq = Sampling_Peak() ;
target_Freq = Locate_Freq(current_Freq);
Peak_Magnitude();


  
}


int Locate_Freq (double current_frequency)
{ 
  int Freq_index;
    for(int i=10;i>=1;i--)
  {
    if(abs(current_frequency-frequency_hz[i])<frequency_diff)
    {
      Freq_index =i;  
      break;
    }
  }
  return frequency_hz[Freq_index]; // return target Freq
}



