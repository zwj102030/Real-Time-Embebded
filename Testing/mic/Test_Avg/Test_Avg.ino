#include "arduinoFFT.h"
 
#define SAMPLES 256             //Must be a power of 2
#define SAMPLING_FREQUENCY 25600 //Hz, must be less than 10000 due to ADC
 
arduinoFFT FFT = arduinoFFT();
 
unsigned int sampling_period_us;
unsigned long microseconds;
 
double vReal[SAMPLES];
double vImag[SAMPLES];
 
void setup() {
    Serial.begin(115200);
 
    sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
}
 
void loop() {

    int startTime = millis();
    double freqReadings[10];
    static double totReadings[10];

    for(int i = 0; i < 10; i++)
    {
      totReadings[i] = 0;
    }

    for(int i = 0; i < 31; i++)
    {

        /*SAMPLING*/
      for(int j=0; j<SAMPLES; j++)
      {
          microseconds = micros();    //Overflows after around 70 minutes!
       
          vReal[j] = analogRead(A0);
          vImag[j] = 0;
       
          while(micros() < (microseconds + sampling_period_us)){
          }
      }
    
      /*FFT*/
      FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
      FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
      FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    
      /*PRINT RESULTS*/

      for(int j = 0; j < 10; j++)
        freqReadings[j] = (0.5 * vReal[49+5*j]) + vReal[50+5*j] + (0.5 * vReal[51+5*j]);

      for(int j = 0; j < 10; j++)
      {
        totReadings[j] += freqReadings[j];
      }

      delay((39 * (i+1)) - (millis()-startTime));
    }
   


    for(size_t i = 0; i < 10; i++)
    {
      Serial.print(5000 + (500 * i)); Serial.print(" Amplitude: "); 
      Serial.println(totReadings[i]);
    }
    Serial.print("\n\n");
    Serial.print("This took "); Serial.print(millis() - startTime); Serial.println(" milliseconds");
    delay(5000);
   
}
