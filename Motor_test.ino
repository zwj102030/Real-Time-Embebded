#include <Servo.h>

Servo motore1;  // Viene creato l'oggetto per il controllo del motore 1
Servo motore2;  // Viene creato l'oggetto per il controllo del motore 2

void setup() {
  motore1.attach(4);  // Collega il motore al pin D3
  motore2.attach(5);  // Collega il motore al pin D4
}

void loop() {
  // Fa ruotare i motori in senso antiorario
  motore1.writeMicroseconds(1000);
  motore2.writeMicroseconds(1000);
  delay(1000);
   // Motori fermi
  motore1.writeMicroseconds(1500);
  motore2.writeMicroseconds(1500);
  delay(1000);
   // Fa ruotare i motori in senso antiorario orario
  motore1.writeMicroseconds(2000);
  motore2.writeMicroseconds(2000);
  delay(1000);
     // Motori fermi
  motore1.writeMicroseconds(1500);
  motore2.writeMicroseconds(1500);
  delay(1000);
}
