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

    for(int i = 0; i < 15; i++)
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
//      freqReadings[0] = vReal[25]; //5000 Hz
//      freqReadings[1] = ((vReal[27] + vReal[28]))/2; //5500Hz
//      freqReadings[2] = vReal[30]; //6000 Hz
//      freqReadings[3] = ((vReal[32] + vReal[33]))/2; //6500Hz
//      freqReadings[4] = vReal[35]; //7000 
//      freqReadings[5] = ((vReal[37] + vReal[38]))/2; //7500Hz
//      freqReadings[6] = vReal[40]; //8000 
//      freqReadings[7] = ((vReal[42] + vReal[43]))/2; //8500Hz
//      freqReadings[8] = vReal[45]; //9000 
//      freqReadings[9] = ((vReal[47] + vReal[48]))/2; //9500Hz

      freqReadings[0] = vReal[50];
      freqReadings[1] = vReal[55];
      freqReadings[2] = vReal[60];
      freqReadings[3] = vReal[65];
      freqReadings[4] = vReal[70];
      freqReadings[5] = vReal[75];
      freqReadings[6] = vReal[80];
      freqReadings[7] = vReal[85];
      freqReadings[8] = vReal[90];
      freqReadings[9] = vReal[95];

      for(int j = 0; j < 10; j++)
      {
        totReadings[j] += freqReadings[j];
      }

      delay((62.5 * (i+1)) - (millis()-startTime));
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
