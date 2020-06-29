// Ultrasonic - Library for HR-SC04 Ultrasonic Ranging Module.
// Rev.4 (06/2012)
// J.Rodrigo ( www.jrodrigo.net )
// more info at www.ardublog.com

#include <Ultrasonic.h>
#include <TCS3200.h>

//-----ultrasonic----------

Ultrasonic ultraRight(A1,A0);   // (Trig PIN,Echo PIN)
Ultrasonic ultraMid(A3,A2);  // (Trig PIN,Echo PIN)
Ultrasonic ultraLeft(A5,A4);

//------------------------

//-----colorSens----------
uint8_t RGBvalue[3];

TCS3200 colSens;

//------------------------

void setup() {
  Serial.begin(9600); 
//--colorsens--

colSens.begin();
---------------
}

//===----ColorSens----===

 
colSens.loop();
colSens.getRGB (RGBvalue);

=====-----------------====



void loop()
{
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
