#include<Wire.h>
int x=0;
int current=0;
int prev=0;
void setup() {
  // put your setup code here, to run once:
pinMode(9,OUTPUT);
pinMode(8,OUTPUT);
Wire.begin(7);
Wire.onReceive(receiveEvt);

}
void receiveEvt(int bytes){
   x=Wire.read();  
  
  }

void wait(int i){
  long times=millis()+i;
  while(millis()<times){
    
    }
  }


void loop() {
  // put your main code here, to run repeatedly:
//analogWrite(9,x);

//map(throttle, 0, 1023, 0, 500);
digitalWrite(8,HIGH);

wait(x);

   
   digitalWrite(8,LOW);
   
wait(x);
  
}
