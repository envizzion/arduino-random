#include <avr/io.h>
#include <avr/interrupt.h>
#include <TCS3200.h>
//=======================VARIABLES=========================


unsigned long times=0;

//-----caliberation--------
volatile byte beginRun=0; 
int maxx=0,minn=0,avg,avgPWM;

byte controlPins[] = {B00000000, 
                  B00000010,
                  B00000100,
                  B00000110,
                  B00001000,
                  B00001010,
                  B00001100,
                  B00001110,
                  B00010000,
                  B00010010,
                  B00010100,
                  B00010110,
                  B00011000,
                  B00011010,
                  B00011100,
                  B00011110 }; 
 
// holds incoming values from 74HC4067                  
int muxValues[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};

//-----------------------------

//-----------NRF--------------
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
Servo variable,value;
unsigned long lastRecvTime = 0,now,curr;
const uint64_t pipeIn = 0xE8E8F0F0E1LL;
RF24 radio(9, 10); 

//We could use up to 32 channels
struct MyData {
uint16_t throttle; //We define each byte of data input, in this case just 6 channels
uint16_t yaw;
uint16_t pitch;
uint16_t roll;
uint16_t AUX1;
uint16_t AUX2;
};

MyData data;


//----------------------------


//---------internal Comm----------


volatile byte last_channel_1, last_channel_2;
volatile unsigned long timer_1, timer_2,current_time;
volatile int r1,r2,count;
byte colorTask=0;
volatile byte boxTask=0;
//--------------------------------

//---------colorSens--------------

uint8_t RGBvalue[3];
TCS3200 colSens;

//--------------------------------

//===============================================================

//==========================METHODS==============================



void wait(unsigned long i){
 
   times=micros()+i;
      while(micros()<times){
   // Serial.println(micros());
    }
}


//=====----caliberation----=====
void initCali(){

DDRC = B00111110; 

DDRD |=(1<<3);

//TCCR2A=0x00;
TCCR2B=0x00;

TCCR2A |= (1<<COM2B1|1<<WGM21|1<<WGM20);
TCCR2B |= (1<<CS20);

  }

void setPin(int outputPin)
// function to select pin on 74HC4067
{
  PORTC = controlPins[outputPin];
}


void getIRMinMax()

{
  
  for (int i = 0; i < 16; i++)
  {
    if(muxValues[i]<minn)minn=muxValues[i];
    if(muxValues[i]>maxx)maxx=muxValues[i];
  }
  
  avg=(minn+maxx)/2;
  avgPWM=(avg/4)+5;
  
}


//=====-------------------=====  


//=====---------NRF----------=====  


void resetData()
{
//We define the inicial value of each data input
//3 potenciometers will be in the middle position so 127 is the middle from 254
data.throttle = 1000;
data.yaw = 127;
data.pitch = 1500;
data.roll = 1500;
data.AUX1 = 0;
data.AUX2 = 0;

}


void initNRF(){
  
//pinMode(A0,OUTPUT);
pinMode(A5,OUTPUT);

resetData();
radio.begin();
radio.setAutoAck(false);
radio.setDataRate(RF24_250KBPS);

radio.openReadingPipe(1,pipeIn);
//we start the radio comunication
radio.startListening();
//variable.attach(A0);
//value.attach(A5);
//right.attach(5);
//left.write(90);
//right.write(90);


}






void recvData()
{
while ( radio.available() ) {
radio.read(&data, sizeof(MyData));
lastRecvTime = millis(); //here we receive the data
  
  
  }
}
void setPID_PWM(){
  
  recvData();
unsigned long now = millis();
unsigned long curr=micros();
//Here we check if we've lost signal, if we did we reset the values 
if ( now - lastRecvTime > 1000 ) {
// Signal lost?
resetData();



  
  
  }
  variable.writeMicroseconds(data.throttle+1000);
  value.writeMicroseconds(data.yaw+1000);

}

//=====-----------------------------=====  



//=====---internal Communication----========
void initComm(){
   
   PCICR |= (1 << PCIE1);                                       //Set PCIE0 to enable PCMSK0 scan.
   PCMSK1 |= (1 << PCINT8);      //A1 reads receiver
   pinMode(A5,OUTPUT);
  
  
  }


ISR(PCINT1_vect){
  current_time = micros();

  //Channel 1=========================================
  if(PINC & B00000001){                                        //Is input 8 high?
    if(last_channel_1 == 0){                                   //Input 8 changed from 0 to 1
     last_channel_1 = 1;                                      //Remember current input state
      timer_1 = current_time;                                  //Set timer_1 to current_time
    count++;
    }
  }
  else if(last_channel_1 == 1){                                //Input 8 is not high and changed from 1 to 0
    last_channel_1 = 0;                                        //Remember current input state
    r1 = current_time - timer_1;                //Channel 1 is current_time - timer_1
   // updateTask(r1);


      if(r1<=1600 && r1>=1400) {
          boxTask=1;
       }  
      else if(r1<=2100 && r1>=1900){
          colorTask=1;
       } 
  
  }
}


void transmit(int pulse){

   PORTC |=(1<<5);
   wait(pulse);
   PORTC &=~(1<<5);
   wait(4000);
  }

//===========--------------------============

  
//======----------ColorDetection-------======
 void getColor(){
  unsigned long tme=millis()+2000;
  while(tme>=millis()){
       colSens.loop();
      
    }
    colSens.getRGBtoMaxCorrection (RGBvalue);
    if(RGBvalue[0]==255)Serial.println("R"); 
    else if(RGBvalue[1]==255)Serial.println("G"); 
    else Serial.println("B"); 
  
  }


//=====------------------------------========


//====================================================
  
void setup() {
  // put your setup code here, to run once:
//--------caliberation-------
pinMode(5,OUTPUT);
digitalWrite(5,LOW);
initCali();
initNRF();
//Serial.begin(9600);

while(millis()<5000){
    for(int i=8;i<16;i++){
    setPin(i);
    muxValues[i]=analogRead(A7);

}
    getIRMinMax();
    
 }
OCR2B=avgPWM;
digitalWrite(5,HIGH);


 while(1){
  if(digitalRead(8)==LOW){
      digitalWrite(1,LOW);
      pinMode(A0,OUTPUT);
      digitalWrite(A0,HIGH);
       delay(2000);
       digitalWrite(A0,LOW);
      pinMode(A0,INPUT);
      
      break;
      
    }
Serial.println("button works");
 }




//---------------------------
initComm();
sei();
}

void loop() {
  // put your main code here, to run repeatedly:
 //setPID_PWM();

 if(colorTask){
  getColor();
  
  
  
  
  }
 delay(500);
}
