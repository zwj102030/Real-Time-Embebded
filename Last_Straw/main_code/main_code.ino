#include "arduinoFFT.h"
//#include "Motion.h"
#include <Servo.h>
#include <Ping.h>

#define trigPin 21 //A9
#define echoPin 20 //A8




extern double get_distance()  // returns distance in Centimeters but i havent tested the exact accuracy
    {
      long duration, distance; 
      digitalWrite(trigPin, LOW); 
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
    
      delayMicroseconds(10); 
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance = (duration/2) / 29.1;
      return distance;
    
    }
  



Servo motore1;  
Servo motore2;
enum Dir { front, back ,left, right, v_stop };
arduinoFFT FFT = arduinoFFT();


int FREQ_THRESHOLDS[10]={5000,5500,6000,6500,7000,7500,8000,8500,9000,9500}; //our main frequency array
int FREQ_AMP_THRESHOLDS[10]; 
int Freq_threshold= 250;
short beenToFreq[10]; // an array which acts like a flag for each frequency which has been visited 



//for the last_straw function
#define frequency_diff 20//the max variation of the frequncy

#define straight_distance 100

#define distance_Threshold 15//if the distance is larger than this, it will go

#define SAMPLES 128             //Must be a power of 2
#define SAMPLING_FREQUENCY 25600 //Hz, must be less than 10000 due to ADC
 
unsigned int sampling_period_us;
unsigned long microseconds;
 
double vReal[SAMPLES];
double vImag[SAMPLES];
 
void setup() {
  // put your setup code here, to run once:
      Serial.begin(115200);
      motore1.attach(A9);  // left
      motore2.attach(A8);  // right wheel  
      pinMode(trigPin, OUTPUT);
      pinMode(echoPin, INPUT);
 
      sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));

}

void loop() {
  // put your main code here, to run repeatedly:
  last_straw(); //the function which will look for the last frequency and move towards it; while avoiding obstacles 
  // scan(); takes in the ten frequency bands
  //double* frequency = freq(); //returns the pointer to the array which stores the amplitudes of our 10 required bands
  delay(1000);
  

}

void drive_verichel (char direct, char angle_rad , char distance_cm ) 
{
  switch ( direct) 
  {
    case front : 
        
        motore1.writeMicroseconds(1300); //left forward
        motore2.writeMicroseconds(1283);  //right backward 
        delay(1800*distance_cm); // distance
        
        stop_function();
        break ;
    case back : 
        motore1.writeMicroseconds(1300); //left turn around 
        motore2.writeMicroseconds(1800);  //right backward 
        delay(1800*angle_rad); // distance
        motore1.writeMicroseconds(1300); // left forward
        motore2.writeMicroseconds(1283);  //right backward 
        delay(1800*distance_cm); // distance
        stop_function();
        break ;
    case left : 
        motore1.writeMicroseconds(1300); //left forward
        motore2.writeMicroseconds(1800);  //right backward 
        delay(1800*angle_rad); // angle
        stop_function();
        break ;
    case right : 
        motore1.writeMicroseconds(1800); //left forward
        motore2.writeMicroseconds(1300); //right backward 
        delay(1800*angle_rad); // angle
        stop_function();
        break ;
    case v_stop: 
       stop_function();
        break ;
  }
}
void stop_function()
{
        motore1.writeMicroseconds(1500); // left forward
        motore2.writeMicroseconds(1500);  //right backward 
}



void scan()
{
//  delay(1000);
int highestFreq[10]; //Lists the sample number where the highest amplitude was found for a given frequency
double highestFreqMag[10]; //Highest amplitude found for the given frequency
double* freqReadings; //Frequency readings from current angle
                        //int FREQ_AMP_THRESHOLDS[10]; //Constant threshold values: If higher, can go to it
                        //short beenToFreq[10]; //Array that holds whether the car has been to a frequency
int direction = 0; //Holds which sample to go towards
int num_samples =8;
      for (size_t i = 0; i < num_samples; i++)
      {
          
          freqReadings = freq();
          delay(1000);
          
        for(size_t j = 0; j < 10; j++)
        {
          Serial.println(j);
          Serial.println(freqReadings[j]);
          Serial.println("\n\n\n\n\n\n\n");
          if(freqReadings[j] > highestFreqMag[j])
          {
            highestFreq[j] = i;
//            Serial.println(j);
            highestFreqMag[j] = freqReadings[j];
          }
        }
      }
      

        for (size_t i = 9; i > -1; i--)
        {
          if (highestFreqMag[i] >= FREQ_AMP_THRESHOLDS[i] && beenToFreq[i] != 1)
          {
            direction = highestFreq[i];
            break;
          }
        }
}


double* freq() {

   static double freqReadings[10];

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
    
//    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
//    Serial.println(peak);
    

    //FFT stuff goes here
    
//    (6000) / (25600/128)
    
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
    
    return freqReadings;
   
}

double peak_measure()
{
  for(int i=0; i<SAMPLES; i++)
    {
        microseconds = micros();    //Overflows after around 70 minutes!
     
        vReal[i] = analogRead(A0);
        vImag[i] = 0;
     
        while(micros() < (microseconds + sampling_period_us)){
        }
    }
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
    return peak;
    
}


bool collision()
{
  if(get_distance() < distance_Threshold )
  {
    return false;
  }
  return true;
}


extern void light_led();//light the led when find the last mine
bool Straight_avoid(int vec = 50, int dtime = 700) {
  unsigned long microseconds = micros();    //Overflows after around 70 minutes!
  drive_verichel(front,0,10);
  while (micros() < (microseconds + dtime*1000)) {
    if (get_distance() <= distance_Threshold) {
      stop_function();
      return false;
    }
  }
  stop_function();
  return true;
}

void last_straw()
{
  while(get_distance()>distance_Threshold)
  {
    drive_verichel(front,0,10);//10 cm move forward
   
    double max_turn[8]; // 8 stops in our 360 turn
    double max_p =0;
    int index_freq=0;
    for(int turn=0;turn<8;turn++) // our 360 degree turn with 8 stops
    {
    
          drive_verichel(right, 0.8,0); //45 degrees = 0.8 rad
          
                  
                  
                  
                  double *p = freq();  //returns a pointer to our frequency array 
                  //i have a doubt regarding this frequency reading time, 
                  //since we want our microphone to read the frequencies over the entire second, 
                  //how are we suppose to implement that
                  //i can't just give a delay function here, 
                  for(int i=0;i<10;i++)
                  {
                    if (*(p+i)>max_p)
                    {
                      max_turn[turn]=*(p+i); // and array which stores the maximum of each turn 
                      max_p=*(p+i);
                      index_freq=i;
                    }
                  }
                  
                 delay(100);// pls put delay functions wherever necessary

     }     

            //after one rotation, we have an array max_turn[8] which has the maximum values for each turn
            double temp=0;
            double max_value =0;
            int max_value_index =0; // for the highest value among the 8 stops in our turn
            for(int j=0;j<8;j++)
            { 
              if(max_turn[j]>max_value)
              {
                max_value=max_turn[j];
                max_value_index=j;
              }
            }

            switch (max_value_index)
            {
              case 0:
                  {
                    drive_verichel(front,0,10);
                    last_straw();
                    
                  }

               case 1:
                  {
                    drive_verichel(right,0.8,10);
                    drive_verichel(front,0,10);
                    last_straw();
                    
                  }
               case 2:
                  {
                    drive_verichel(right,1.6,10);
                    drive_verichel(front,0,10);
                    last_straw();
                    
                  }
               case 3:
                  {
                    drive_verichel(right,2.3,10);
                    drive_verichel(front,0,10);
                    last_straw();
                    
                  }    
               case 4:
                  {
                    drive_verichel(right,3.14,10);
                    drive_verichel(front,0,10);
                    last_straw();
                    
                  }   
               case 5:
                  {
                    drive_verichel(left,2.3,10);
                    drive_verichel(front,0,10);
                    last_straw();
                    
                  }     
                case 6:
                  {
                    drive_verichel(left,1.6,10);
                    drive_verichel(front,0,10);
                    last_straw();
                    
                  }  
                case 7:
                  {
                    drive_verichel(left,0.8,10);
                    drive_verichel(front,0,10);
                    last_straw();
                    
                  } 
                 case 8:
                 {
                    drive_verichel(front,0,10);
                    last_straw();    
                 }
                 default:last_straw();
                 
                     
                  
                  
            }

    
  }
  //this is for if the beacon is too close i.e the get_distance returned something less than 15 cm (our distance threshold)

  drive_verichel(right,1.6,0); //turn right 90 degrees
  drive_verichel(front,0,45);  // drive for 45 cm
  drive_verichel(left,1.6,0);  // turn left 45 degrees
  drive_verichel(front,0,45);  // drive for 45 cm
  last_straw();

  //need to add the area where we amend the beenToFreq[10]; function
  //need to add a default condition for if our vehicle reaches the 9500Hz frequency beacon
}


// i haven't incorporated this avoid function anywhere 

void avoid(double max_frequency) 
{ 
  while (1) 
  {
    drive_verichel(right,45,0); //right rotate
    if (get_distance() > distance_Threshold) 
      {
         drive_verichel(front,0,straight_distance);//straight to right
      }
    else 
    {
      drive_verichel(left,45,0);
        if (!Straight_avoid(50, straight_distance)) 
        {
          if (abs(max_frequency - 10000) < frequency_diff) 
          {
            break;
          }
          drive_verichel(left,45,0);
          continue; //situation 2
        }
    }
    delay(500);

     drive_verichel(left,45,0); //left rotate
        if (get_distance() > distance_Threshold) 
        {
          drive_verichel(front,0,straight_distance);//straight to left
          break;
        }
     else 
     {
        if (!Straight_avoid(50, straight_distance))
        {
          if (abs(max_frequency - 10000) < frequency_diff) {
            break;
         }
      continue;//situation 3
    }
    delay(500);

   
    delay(500);
  }
  }}



