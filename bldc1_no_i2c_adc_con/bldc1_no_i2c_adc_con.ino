



















































#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/twi.h>
#include<Wire.h>
#include <atmega-adc.h>



#define AA1  DDRB |= (1<<5);
#define AA1_ON   PORTB |= (1<<5);
#define AA1_OFF PORTB &= ~(1<<5);

#define AA2  DDRB |= (1<<4);
#define AA2_ON PORTB |= (1<<4);
#define AA2_OFF  PORTB &= ~(1<<4);



#define BB1  DDRB |= (1<<3);
#define BB1_ON  PORTB |= (1<<3);
#define BB1_OFF  PORTB &= ~(1<<3);

#define BB2  DDRB |= (1<<2);
#define BB2_ON   PORTB |= (1<<2);
#define BB2_OFF  PORTB &= ~(1<<2);


#define CC1  DDRB |= (1<<1);
#define CC1_ON   PORTB |= (1<<1);
#define CC1_OFF  PORTB &= ~(1<<1);

#define CC2  DDRB |= (1<<0);
#define CC2_ON   PORTB |= (1<<0);
#define CC2_OFF  PORTB &= ~(1<<0);



#define PHASE_ALL_OFF  AA1_OFF;AA2_OFF;BB1_OFF;BB2_OFF;CC1_OFF;_OFF;











// Define various ADC prescaler
const unsigned char PS_8 = (1 << ADPS1)| (1 << ADPS0);
const unsigned char PS_16 = (1 << ADPS2);
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);








 int emfA=0;
int emfB=0;
int emfC=0;

 volatile int a1,a2,a3,highFlag=0,flag1,minPWM;

 volatile int tmpPWM;
 volatile int maxPWM=1500;
 volatile unsigned long current_time,timer_1;
volatile byte last_channel_1;


volatile static int delta= 0;
volatile static int Lastdelta= -1;
  volatile int emA = 0;
 volatile  int emB =0;
   volatile   int emC =0;
    volatile  int sum =0;  
    volatile  int fase=1;


/*int AA1=13;
int AA2=12;
int BB1=11;
int BB2=10;
int CC1=9;
int CC2=8;*/
int led = 12;
volatile long stepLength = 15000;
volatile int minStepLength = 1;
int steps =2 ;
volatile unsigned long times = 0;
//volatile int flag=1;
void setup() {
//Serial.begin(250000);
 /* pinMode(AA1,OUTPUT);
  pinMode(AA2,OUTPUT);
  pinMode(BB1,OUTPUT);
  pinMode(BB2,OUTPUT);
  pinMode(CC1,OUTPUT);
  pinMode(CC2,OUTPUT);*/
 // pinMode(emfA,INPUT);
 // pinMode(emfB,INPUT);
 // pinMode(emfC,INPUT);
  //Wire.begin(7);
  //Wire.onReceive(receiveEvt);
AA1;AA2;BB1;BB2;CC1;CC2;
 ADCSRA &= ~PS_128;
 //ADCSRA |=PS_8; 

 ADCSRA |= PS_16;


a1=0;a2=0;a3=0;
adc_start(ADC_PRESCALER_16, ADC_VREF_AVCC, 3, myfunction);


PCICR |= (1 << PCIE2);                                       //Set PCIE0 to enable PCMSK0 scan.
  PCMSK2 |= (1 << PCINT16);  
  // enable global interrupts
  sei();
//flag=0;


 while(millis()<3000){
    current_time = micros();

  //Channel 1=========================================
 
  if(PIND & B00000001){                                        //Is input 8 high?
    if(last_channel_1 == 0){                                   //Input 8 changed from 0 to 1
      last_channel_1 = 1;                                      //Remember current input state
      timer_1 = current_time;                                  //Set timer_1 to current_time
    
    }
  }
  else if(last_channel_1 == 1){                                //Input 8 is not high and changed from 1 to 0
    last_channel_1 = 0;                                        //Remember current input state
     //tmpPWM= current_time - timer_1;                //Channel 1 is current_time - timer_1
    tmpPWM=current_time - timer_1;
  }
 
  
   if(tmpPWM>=maxPWM){
      maxPWM=tmpPWM;
    }
 /*  if(tmpPWM<=minPWM){
     minPWM=tmpPWM;
    }*/
    
 }
    last_channel_1=0;
    //minPWM=maxPWM-1000;
    sei();
}  


ISR(PCINT2_vect){
  current_time = micros();

  //Channel 1=========================================
  if(PIND & B00000001){                                        //Is input 8 high?
    if(last_channel_1 == 0){                                   //Input 8 changed from 0 to 1
      last_channel_1 = 1;                                      //Remember current input state
      timer_1 = current_time;                                  //Set timer_1 to current_time
    }
  }
  else if(last_channel_1 == 1){                                //Input 8 is not high and changed from 1 to 0
    last_channel_1 = 0;                                        //Remember current input state
    tmpPWM= current_time - timer_1;                //Channel 1 is current_time - timer_1
    minStepLength=1+maxPWM-tmpPWM;               //Channel 1 is current_time - timer_1
  }
}


// void receiveEvt(int bytes){
//     if(flag==1){  

//    minStepLength=Wire.read(); 
 //   flag=0; 
  
//  }
// }

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
   
      
     
  }
}



void switchStep(int stage)
{
  switch(stage)
  {





    //Phase1 C-B
 
    case 1:
     //AA1_OFF;
      AA2_OFF;
     // BB1_OFF;
      //CC2_OFF;
      if(highFlag==0){CC1_ON;highFlag==1;}
      else{CC1_OFF;highFlag==0;}
      BB2_ON;
      //myDelay(stepLength);
      delta = emA-sum;
      
      break;

  //Phase2 A-B
    case 2:
      CC1_OFF;
     // AA2_OFF;
     // BB1_OFF;
     // CC2_OFF;
     // AA1_ON;

      if(highFlag==0){AA1_ON;highFlag==1;}
      else{AA1_OFF;highFlag==0;}
      
      BB2_ON;
      //myDelay(stepLength);
       delta = emC-sum;
      break;

  //Phase3 A-C
    case 3: 
       BB2_OFF;
      //CC1_OFF;
     // AA2_OFF;
     // BB1_OFF;
     // AA1_ON;
      if(highFlag==0){AA1_ON;highFlag==1;}
      else{AA1_OFF;highFlag==0;}
      
      CC2_ON;
      delta = emB-sum;
    break;   
  
  //Phase4 B-C
  case 4:
      AA1_OFF;
     // AA2_OFF;
      
      //BB2_OFF;
      //CC1_OFF;
      //BB1_ON;

      if(highFlag==0){BB1_ON;highFlag==1;}
      else{BB1_OFF;highFlag==0;}
      CC2_ON;
      //myDelay(stepLength);
      delta = emA-sum;
      break;

  //Phase5 B-A 
  case 5:
  //AA1_OFF;
      CC2_OFF;
     // BB2_OFF;
      //CC1_OFF;
      //BB1_ON;
      if(highFlag==0){BB1_ON;highFlag==1;}
      else{BB1_OFF;highFlag==0;}
      
      AA2_ON;
      //myDelay(stepLength);
      delta = emC-sum;
      break;

  //Phase6 C-A
  case 6:
   //AA1_OFF;
      
      BB1_OFF;
     // CC2_OFF;
      //BB2_OFF;
     // CC1_ON;
      if(highFlag==0){CC1_ON;highFlag==1;}
      else{CC1_OFF;highFlag==0;}
      
      AA2_ON;
      //myDelay(stepLength);
       delta = emB-sum;
  break;
  }  
   



}








void loop() {
  flag1 = 0;
 // Serial.println(minStepLength);


        //sum = (a1+a2+a3)/3;
 //Serial.print(emA);Serial.print("  ");Serial.print(emB);Serial.print("  ");Serial.print(emC);Serial.print("  ");Serial.print(+delta);Serial.println("");


/*if (flag==1){
//Serial.print(a1);   Serial.print("  "); Serial.print(a2);  Serial.print("  "); Serial.print(a3);  Serial.println(""); 
 sum = (a1+a2+a3)/3;
  
  flag==0;
*/


if(stepLength<1000){



  if (Lastdelta < 0){
  if (delta > 0)
      {
        Lastdelta=delta; //save the last delta
        fase= fase + 1;
        if (fase > 6) {
          fase = 1;
          
          }
        // flag1=1; 
          
      }
  }
   if (Lastdelta > 0){
  if (delta < 0)
      {
        Lastdelta=delta;
        fase= fase + 1;
        if (fase > 6) {
          fase = 1;
          }
         flag1=1;
      }
  }
 


}

else {
  
    if (fase<6){
    fase=fase+1;
     
    }
  if (fase==5){
    fase=1;
  stepLength=990;
  }
 // flag1=1;
   
}
  
  
  


  
  

 
  //myDelay(stepLength);
 // switchStep(2); 
 // switchStep(3);
 // switchStep(4);
  //switchStep(5);
 // switchStep(6);
if(flag1==1){

wait(stepLength);
  if(stepLength > minStepLength)
  {
    stepLength = stepLength - steps;
  }
 else if(stepLength < minStepLength){
  stepLength=stepLength+steps;
   
 }  
  else {
    // set the minimum pulse length
    stepLength=minStepLength;
    
  }
  

   
  if (stepLength < 20000) {
//digitalWrite(led, LOW);      // third gear
    steps = 50;
  }

  if (stepLength < 2000) {
   // digitalWrite(led, HIGH);      // fourth gear
    steps =1;
  }
 //flag1=0;
}
  emA = a1;
  emB = a2;
  emC = a3;
  sum = (emA+emB+emC)/3;

switchStep(fase);
}

  
 






