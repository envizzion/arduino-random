
#include <Stepper.h>
int cw=2;
int ccw=3;
int steps=200;
int enable=8;
Stepper motor(steps,10,11,12,13);


void setup() {
  // put your setup code here, to run once:
pinMode(cw,INPUT);
pinMode(ccw,INPUT);
pinMode(enable,OUTPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
int speed=analogRead(A0);
int rpm=map(speed,0,1023,0,100);
int fwd=digitalRead(cw);
int  reverse=digitalRead(ccw);
digitalWrite(enable,LOW);
if(fwd==1 && reverse==0 && rpm>5){
  
  digitalWrite(enable,HIGH);
  motor.step(1);
  motor.setSpeed(rpm);
  delay(10);
  
  
  }

if(fwd==0 && reverse==1 && rpm>5){
  
  digitalWrite(enable,HIGH);
  motor.step(-1);
  motor.setSpeed(rpm);
  delay(10);
  
  
  }
  Serial.println("rpm :")
  ;
  Serial.print(rpm);

  
}
