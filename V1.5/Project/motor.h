#include <Servo.h>
Servo motore1;  // Viene creato l'oggetto per il controllo del motore 1
Servo motore2;  // Viene creato l'oggetto per il controllo del motore 2
enum Dir { front, back ,left, right, v_stop };
void motor_initial () {
  motore1.attach(A8);  // left
  motore2.attach(A9);  // right wheel  
}
void stop_function()
{
        motore1.writeMicroseconds(1500); // left foreward
        motore2.writeMicroseconds(1500);  //right backward 
}
void drive_verichel (char direct, char delay_time =0 )  //1800 around 1 cc 
{
  switch ( direct) 
  {
    case front : 
        motore1.writeMicroseconds(1300); //left foreward
        motore2.writeMicroseconds(1300);  //right backward 
        if (delay_time !=0 )
        {
        delay(100*delay_time); // distance
        stop_function();
        }
        break ;
    case back : 
        motore1.writeMicroseconds(2000); //left turn around 
        motore2.writeMicroseconds(2000);  //right backward 
         if (delay_time !=0 )
        {
        delay(100*delay_time); // distance
        stop_function();
        }
        break ;
    case left : 
        motore1.writeMicroseconds(1300); //left foreward
        motore2.writeMicroseconds(1700);  //right backward 
         if (delay_time !=0 )
        {
        delay(100*delay_time); // distance
        stop_function();
        }
        break ;
    case right : 
        motore1.writeMicroseconds(1700); //left foreward
        motore2.writeMicroseconds(1300); //right backward 
         if (delay_time !=0 )
        {
        delay(100*delay_time); // distance
        stop_function();
        }
        break ;
    case v_stop: 
       stop_function();
        break ;
  }
}



