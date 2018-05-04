#include "arduinoFFT.h"
 
#define SAMPLES 128             //Must be a power of 2
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

    static double freqReadings[10];
   
    /*SAMPLING*/
    for(int i=0; i<SAMPLES; i++)
    {
        microseconds = micros();    //Overflows after around 70 minutes!
     
        vReal[i] = analogRead(A0);
        vImag[i] = 0;
     
        while(micros() < (microseconds + sampling_period_us)){
        }
    }
 
    /*FFT*/
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
 
    /*PRINT RESULTS*/
    freqReadings[0] = vReal[25]; //5000 Hz
    freqReadings[1] = ((vReal[27] + vReal[28]))/2; //5500Hz
    freqReadings[2] = vReal[30]; //6000 Hz
    freqReadings[3] = ((vReal[32] + vReal[33]))/2; //6500Hz
    freqReadings[4] = vReal[35]; //7000 
    freqReadings[5] = ((vReal[37] + vReal[38]))/2; //7500Hz
    freqReadings[6] = vReal[40]; //8000 
    freqReadings[7] = ((vReal[42] + vReal[43]))/2; //8500Hz
    freqReadings[8] = vReal[45]; //9000 
    freqReadings[9] = ((vReal[47] + vReal[48]))/2; //9500Hz


    for(size_t i = 0; i < 10; i++)
    {
      Serial.print(5000 + (500 * i)); Serial.print(" Amplitude: "); 
      Serial.println(freqReadings[i]);
    }
    Serial.print("\n\n");
    delay(1000);
   
}
