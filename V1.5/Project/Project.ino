#include <Servo.h>
#include "motor.h"
#include "FFT.h"
#include "ultra_sonic.h"
//SAMPLES 128            Must be a power of 2
//SAMPLING_FREQUENCY 25600 Hz, must be less than 80000 due to ADC
#define Volume_Thershold 15//if the Volume is lager than this, it will go
#define distance_Thershold 40//if the distance is lager than this, it will go
#define frequency_diff 200//the max variation of the frequncy

//int frequency_hz[10]={10000,9000,8000,7000,6000,5000,4000,3000,2000,1000};//the frequency of the ten mines
int distance;
int Locate_Freq (double current_frequency);

double Freq_Value_Last;
double Freq_Value_Current;
void print_mic_info ();
double Mag[3] = {0,0,0};
int index_i = 1;
bool max_flag =0;
int counter_i =0;
void setup()
{
  FFT_init() ;
  motor_initial () ;
  ultra_sonic_setup();
  Serial.begin(115200);
}

void loop()
{   
     
  intial_array();
  check_direction();
  
   
 }



void print_info ()
{distance =get_distance();
  Serial.print("Distance "); Serial.print(distance); Serial.println(" cm");
  for(size_t i = 0; i < 10; i++)
    {
      Serial.print(5000 + (500 * i)); Serial.print(" Amplitude: "); 
      Serial.println(totReadings[i]);
    }
    
     Serial.print("\n index:  ");
       Serial.print(index_i);
    Serial.print("\n Marg[0]:  ");
     Serial.print(Mag[0]);
      Serial.print(" Marg[1]:  ");
     Serial.print(Mag[1]);
      Serial.print("Marg[2]:  ");
     Serial.print(Mag[2]);
      Serial.print("\n ");
    Serial.print("This took "); Serial.print(millis() - startTime); Serial.println(" milliseconds");
    
}

void intial_array()
{
 Mag[0]=0;
 Mag[1]=0;
 Mag[2]=0;
check_index();
Sampling();
Mag[1] =totReadings[index_i];
drive_verichel(right,5);
Sampling();
Mag[0] = totReadings[index_i];
drive_verichel(left,5);
delay(100);
drive_verichel(left,5);
Sampling();
Mag[2] = totReadings[index_i];
drive_verichel(right,5);
}



void  check_direction()
{
  check_index();
   print_info ();
if ((Mag[1]-Mag [0]>=Volume_Thershold) && (Mag[1]-Mag [2]>=Volume_Thershold) )
{
drive_verichel (front,30);
intial_array();
check_direction();
}
else if (Mag[0]-Mag [1]>=Volume_Thershold)
{
drive_verichel (left,5);
Sampling();
Mag[2]=Mag[1];
Mag[1]=Mag[0];
Mag[0] = totReadings[index_i];
check_direction();
}

else if (Mag[2]-Mag [1] >=Volume_Thershold)
{
drive_verichel (right,5);
Sampling();
Mag[0]=Mag[1];
Mag[1]=Mag[2];
Mag[2] = totReadings[index_i];
check_direction();
}
else 
{
 intial_array();
 }
}




void check_index()
{
  for (int i= index_i; i<10;i++)
  {
   if ( totReadings[i]<totReadings [index_i])
      counter_i++;
  }
  if (max_flag==(10-counter_i))
  max_flag =1;
 if (get_distance() <=15 &&max_flag ==1)
 {
   index_i++;
   max_flag=0;
 }
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













