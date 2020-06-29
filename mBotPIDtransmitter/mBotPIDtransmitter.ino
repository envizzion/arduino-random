#include <atmega-adc.h>
int b1,b2,b3,b4;
float a1,a2,a3,a4,times;
int flag=0;
#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*Create a unique pipe out. The receiver has to 
  wear the same unique code*/
  
const uint64_t pipeOut = 0xE8E8F0F0E1LL; //IMPORTANT: The same as in the receiver

RF24 radio(9, 10); // select  CSN  pin

// The sizeof this struct should not exceed 32 bytes
// This gives us up to 32 8 bits channals
struct MyData {
 uint16_t throttle;
 uint16_t yaw;
  uint16_t pitch;
  uint16_t roll;
     byte AUX1;
  byte AUX2;
};

MyData data;

void resetData() 
{
  //This are the start values of each channal
  // Throttle is 0 in order to stop the motors
  //127 is the middle value of the 10ADC.
    
  data.throttle = 0;
  data.yaw = 127;
  data.pitch = 127;
  data.roll = 127;
  data.AUX1 = 0;
  data.AUX2 = 0;
}


void setup() {
  Serial.begin(9600);
 // pitch.attach(11);
 // roll.attach(10);
 // yaw.attach(9);
 // throttle.attach(6);
  DDRB |= B11110000;
  DDRD |= B11110000;
  // put your setup code here, to run once:
 adc_start(ADC_PRESCALER_32, ADC_VREF_AVCC, 4, myfunction);
 
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipeOut);
  resetData();
}


void wait(unsigned long i){
 

 // if (i > 16380) {
 // times=millis()+i/1000;
 // while(millis()<times){
    
 //   }
  //}
 // else{
   times=micros()+i;
      while(micros()<times){
    
    }
    
   // }
}



void myfunction(uint8_t pin, uint16_t value) {

  switch(pin) {
    case(0):
      a1=value;
      break;
      
    case(1):
      a2=value;
      
      break;
      
    case(2):
      // do something else
      a3=value;
//     flag=1;
      break;
   
       case(3):
      // do something else
      a4=value;
      //flag=1;
      break;
     
  
  }
}







void loop() {
  // put your main code here, to run repeatedly:

 


//b1 = ((a1/1024)*180);
//b2 = (a2/1024)*180;
//b3 = (a3/1024)*180;
//b4 = (a4/1024)*180;
b1=a1;
b2=a2;
b3=a3;
b4=a4;


/*

if(b1<1540 && b1>1460){b1=90;}//PORTD |=(1<<7);}
//else{//PORTD &= ~(1<<7);}
if(b2<1540 && b2>1460){b2=90;}//PORTD |=(1<<6);}
//else{//PORTD &= ~(1<<6);}
if(b3<1540 && b3>1460){b3=90;}//PORTD |=(1<<5);}
//else{//PORTD &= ~(1<<5);}
if(b4<1540 && b4>1460){b4=90;}//PORTD |=(1<<4);}
//else{//PORTD &= ~(1<<4);}

if(b1>1900){b1=110;}
if(b2>1900){b2=110;}
if(b3>1900){b3=110;}
if(b4>1900){b4=110;}
*/



Serial.print("kp= ");
Serial.print(b1);
Serial.print("  kd= ");
Serial.print(b2);
Serial.print(" ");
//Serial.print(b3);
//Serial.print(" ");
//Serial.print(b4);
Serial.println("");


 data.throttle =b1;
 data.yaw      = b2;
 data.pitch    = b3;
 data.roll     =b4;
 data.AUX1     = 0; //The 2 toggle switches
 data.AUX2     = 0;

  radio.write(&data, sizeof(MyData));

//pitch.write(a1);
//roll.write(a2);
//yaw.write(a3);
//throttle.write(a4);


/*
PORTD |=(1<<7);
wait(b1);
PORTD &=~(1<<7);

PORTD |=(1<<6);
wait(b2);
PORTD &=~(1<<6);

PORTD |=(1<<5);
wait(b3);
PORTD &=~(1<<5);

PORTD |=(1<<4);
wait(b4);
PORTD &=~(1<<4);

*/
//flag=0;

}

