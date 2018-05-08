#ifndef FFT_H_INCLUDED
#define FFT_H_INCLUDED

#include "arduinoFFT.h"
#define SAMPLES 256             //Must be a power of 2
#define SAMPLING_FREQUENCY 25600//Hz, must be less than 80000 due to ADC
#define threshold_value 150 // for the volume
arduinoFFT FFT = arduinoFFT();
unsigned int sampling_period_us;

typedef struct
{
  double[10] magnitudes;
  short      peakFreq;
} freqData;

void FFT_init();

freqData sampling();

#endif //FFT_H_INCLUDED