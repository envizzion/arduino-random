// Ultrasonic - Library for HR-SC04 Ultrasonic Ranging Module.
// Rev.4 (06/2012)
// J.Rodrigo ( www.jrodrigo.net )
// more info at www.ardublog.com

#include <Ultrasonic.h>

Ultrasonic ultraRight(A1,A0);   // (Trig PIN,Echo PIN)
Ultrasonic ultraMid(A3,A2);  // (Trig PIN,Echo PIN)
Ultrasonic ultraLeft(A5,A4);

void setup() {
  Serial.begin(9600); 
}

void loop()
{
  int x=ultraLeft.Ranging(CM);
  int y=ultraMid.Ranging(CM);
  int z=ultraRight.Ranging(CM);
  Serial.print("Left: ");
  Serial.print(ultraLeft.Ranging(CM)); // CM or INC
  Serial.print(" cm     " );
  delay(50);

   Serial.print("Middle: ");
  Serial.print(ultraMid.Ranging(CM)); // CM or INC
  Serial.print(" cm" );
  delay(50);
  
  Serial.print("Right: ");
  Serial.print(ultraRight.Ranging(CM)); // CM or INC
  Serial.println(" cm" );
  delay(50);
}
