#include<Wire.h>
int x=0;
int a1=0;
int a2=0;
void setup() {
  // put your setup code here, to run once:
Wire.begin();
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(x);//Serial.print("  ");Serial.print(a1);Serial.print("  ");Serial.print(a2);Serial.println("  ");
x=analogRead(A1);
x=map(x, 0, 1023, 0,250);

Wire.beginTransmission(7);
//a1=micros();
Wire.write(x);
//a2=micros();
Wire.endTransmission();

//delay(100);


}



