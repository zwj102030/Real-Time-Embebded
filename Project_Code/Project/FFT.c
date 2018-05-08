void FFT_init() {
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
}

freqData sampling() {
   unsigned long microseconds;
   static double vReal[SAMPLES];
   static double vImag[SAMPLES];
   double Freq_value;
   int startTime ;
   double freqReadings[10];
   static double totReadings[10];

   char index = 0;
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
       freqReadings[i] =   vReal[50+i*5];              //(vReal[50+i*5]+vReal[49+i*5]+vReal[51+i*5])/3;
    }
 
    for (int j = 0; j < 10; j++)
    {
      if ( (freqReadings[j] >=threshold_value) && (freqReadings[j] > totReadings[j]))
        totReadings[j] = freqReadings[j];
      if(freqReadings[j] > freqReadings[index])
        index = j;
    }
    delay((62.5 * (i+1)) - (millis()-startTime));
  }
   for (int i=10;i>0;i--)
   {  
     if(totReadings[i] != 0 && index ==0)
      index = i;
   }
  return (freqReadings, peakFreq);
}