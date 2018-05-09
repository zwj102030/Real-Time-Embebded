#ifndef FFT_H_INCLUDED
#define FFT_H_INCLUDED

#include "arduinoFFT.h"
#define SAMPLES 256             //Must be a power of 2
#define SAMPLING_FREQUENCY 25600//Hz, must be less than 80000 due to ADC
#define threshold_value 150 // for the volume
arduinoFFT FFT = arduinoFFT();
unsigned int sampling_period_us;
unsigned long microseconds;
double vReal[SAMPLES];
double vImag[SAMPLES];
double Freq_value;
char counter ;
int startTime ;
double freqReadings[10];
static double totReadings[10];
void FFT_init() {
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
}
void Sampling (){
 //  int index ;
  startTime = millis();
  
  for (int i = 0; i < 10; i++)
  {
    totReadings[i] = 0;
  }

  for (int i = 0; i < 15; i++)
  {

    /*SAMPLING*/
    for (int j = 0; j < SAMPLES; j++)
    {
      microseconds = micros();    //Overflows after around 70 minutes!

      vReal[j] = analogRead(A0);
      vImag[j] = 0;

      while (micros() < (microseconds + sampling_period_us)) {
      }
    }

    /*FFT*/
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

 
    for (int i=0;i<10;i++)                  
    {
       freqReadings[i] =   (vReal[50+i*5]+vReal[49+i*5]+vReal[51+i*5])/3;
     
      //  vReal[50+i*5];
    }
 
    for (int j = 0; j < 10; j++)
    { if (freqReadings[j] >=threshold_value)
        {if (freqReadings[j] > totReadings[j])
        totReadings[j] = freqReadings[j];
        }
    }
       delay((62.5 * (i+1)) - (millis()-startTime));
  }
  
}

#endif // MYFFT_H_INCLUDED




//
//
//double Peak_Magnitude (double *vD, uint16_t samples, double samplingFrequency)
//{ Freq_value = 0;
//
//  uint16_t IndexOfMaxY = 0;
//  //If sampling_frequency = 2 * max_frequency in signal,
//  //value would be stored at position samples/2
//  for (uint16_t i = 1; i < ((samples >> 1) + 1); i++) {
//    if ((vD[i - 1] < vD[i]) && (vD[i] > vD[i + 1])) {
//      if (vD[i] > Freq_value) {
//        Freq_value = vD[i];
//        IndexOfMaxY = i;
//      }
//    }
//  }
//  return IndexOfMaxY; // return Peak_Freq_Value
//    double delta = 0.5 * ((vD[IndexOfMaxY - 1] - vD[IndexOfMaxY + 1]) / (vD[IndexOfMaxY - 1] - (2.0 * vD[IndexOfMaxY]) + vD[IndexOfMaxY + 1]));
//    double interpolatedX = ((IndexOfMaxY + delta)  * samplingFrequency) / (samples - 1);
//    if (IndexOfMaxY == (samples >> 1)) //To improve calculation on edge values
//      interpolatedX = ((IndexOfMaxY + delta)  * samplingFrequency) / (samples);
//    return interpolatedX;  //return Peak_Freq
//}

//double Magnitude (double *vD, uint16_t samples, double samplingFrequency, double Target_Freq,char averge_i)
//{
//double avg =0;
//IndexOfMaxY =(Target_Freq*samples)/samplingFrequency;
//  if (averge_i !=0)
//    for (int i =0;i<averge_i/2;i++)  //loop half of i
//    {
//      avg +=  vD[IndexOfMaxY+i]
//      avg +=  vD[IndexOfMaxY-i]
//    }
//  else
//    {
//      avg = vD[IndexOfMaxY]  //return if averge 0
//    }
// return avg  //return request Freq - Value
//}



