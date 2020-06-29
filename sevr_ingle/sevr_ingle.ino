#include<Servo.h>
Servo myServo;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
myServo.attach(6);
}

void loop() {
  // put your main code here, to run repeatedly: 
  myServo.write(160);
  delay(2000);
  myServo.write(80);
  delay(2000);

}
