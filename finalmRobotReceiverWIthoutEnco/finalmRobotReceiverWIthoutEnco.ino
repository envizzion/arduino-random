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
  #include "nRF24L01.h"
  #include "RF24.h"
#include "printf.h"
RF24 radio(9,10);

 const uint64_t pipe = 0xE8E8F0F0E1LL;
//0-RED ,1-GREEN,2-BLUE
 uint8_t colors[] = { 0,0,0,0};
 uint8_t complete[] = { 0,0,0};

boolean shoot_success=false;
boolean color_transmitted=false;
boolean wait_for_reply=true;
boolean wait_for_col=true;


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





void initNRF(){
  
// printf_begin();
radio.begin();  //starts nrf with whatever config settings have been set.
//radio.setRetries(15,15);// optionally, increase the delay between retries & # of retries
  
  

radio.printDetails(); 

  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipe);



}







void TX_mode()// TX_mode function......
  {
   
    printf("Sending..."); // for debuggging info
      bool ok = radio.write( colors, 3 ); // sends data
      if (ok) {              // checks if data was propperly received..
        printf("Ok\n\r");    // for debugging
        
        if(colors[0] || colors[1] ||colors[2] || colors[3]>0){color_transmitted=true;} 
       for(int i=0 ;i<3;i++){colors[i]=0;}
        delay(200);
        
      }
      else {
        printf("Failed\n\r"); // for debugging " lets you know other did not received transmittion "
      } 

  }



    
    void await_response() {
       
     radio.openReadingPipe(1,pipe); // closes writing pipe and opens Reading pipe
        radio.startListening();        // sets listening mode so were receiving and we start all over again from the beginning...
   printf("awaiting response");
    while(!shoot_success){
     if(radio.available()){
         
      printf("reading ongoing");
        radio.read(complete,3); 

      for(int i=0;i<3;i++){
        Serial.print(complete[i]);Serial.print(" ");
        if(complete[i])digitalWrite(6,HIGH);
        }
        Serial.println("");
       shoot_success=true;
       
      }
    } 
    radio.stopListening(); // must stop listening before you open a writing pipe  
    radio.openWritingPipe(pipe);
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

while(1){
 if(colorTask){
  getColor();
  break;
 }

 if(boxTask){//getdistance // update colors array
   break;
  }
}

while(!color_transmitted)TX_mode();

await_response();

 shoot_success=false;
 color_transmitted=false;
 wait_for_reply=true; 
 
 if(colorTask)transmit(2000);
 else if(boxTask)transmit(1500); 

 colorTask=boxTask=0;
}
