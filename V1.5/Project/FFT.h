#ifndef FFT_H_INCLUDED
#define FFT_H_INCLUDED

#include "arduinoFFT.h"
#define SAMPLES 256             //Must be a power of 2
#define SAMPLING_FREQUENCY 25600//Hz, must be less than 80000 due to ADC
//#define threshold_value 150 // for the volume
arduinoFFT FFT = arduinoFFT();
unsigned int sampling_period_us;
unsigned long microseconds;
double vReal[SAMPLES];
double vImag[SAMPLES];
double Freq_value;
char counter;
int startTime;
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

  for (int i = 0; i < 32; i++)   //taking 32 samples
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

 
    for (int j=0;j<10;j++)                  
    {
       freqReadings[j] =   (vReal[50+j*5]+vReal[49+j*5]+vReal[51+j*5])/3;
    }
 
    for (int j = 0; j < 10; j++)
    {
       if (freqReadings[j] > totReadings[j])
          totReadings[j] = freqReadings[j];   
    }
       delay((39.0625 * (i+1)) - (millis()-startTime));
  }
  
}

#endif // FFT_H_INCLUDED

