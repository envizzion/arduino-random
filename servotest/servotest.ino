#include<Servo.h>

Servo s1,s2;
void setup() {
  // put your setup code here, to run once:

s1.attach(6); 
s2.attach(7);

}

void loop() {
  // put your main code here, to run repeatedly:
s1.write(40);
delay(1000);
s2.write(170);
delay(3000);
s1.write(20);
delay(1000);
s2.write(120);
delay(3000);

}

