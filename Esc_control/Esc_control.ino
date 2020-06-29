#include <Servo.h>
Servo esc;
int throttlePin = A3;
 
void setup()
{
  Serial.begin(9600);
esc.attach(6);
}
 
void loop()
{
int throttle = analogRead(throttlePin);
throttle = map(throttle, 0, 1023, 2000,1000);
esc.writeMicroseconds(throttle);
Serial.println(throttle);
}

