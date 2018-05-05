 
 
  
  // returned value: interpolated frequency peak apex
  return interpolatedX;



    interpolatedX = ((IndexOfMaxY + delta)  * samplingFrequency) / (samples);
    if (IndexOfMaxY == (samples >> 1)) //To improve calculation on edge values
     double interpolatedX = ((IndexOfMaxY + delta)  * samplingFrequency) / (samples - 1);
  double delta = 0.5 * ((vD[IndexOfMaxY - 1] - vD[IndexOfMaxY + 1]) / (vD[IndexOfMaxY - 1] - (2.0 * vD[IndexOfMaxY]) + vD[IndexOfMaxY + 1]));
