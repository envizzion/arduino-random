



















































#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/twi.h>
#include<Wire.h>




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








int emfA=A0;
int emfB=A1;
int emfC=A2;






static int delta= 0;
      static int Lastdelta= -1;
 int emA = 0;
      int emB =0;
      int emC =0;
      int sum =0;  
      int fase=1;


#define SENSE_U    ADMUX = 0;
#define SENSE_V   ADMUX = 1;
#define SENSE_W   ADMUX = 2;
#define SENSE_H    (ACSR&(1<<ACO))




/*int AA1=13;
int AA2=12;
int BB1=11;
int BB2=10;
int CC1=9;
int CC2=8;*/
int led = 12;
volatile long stepLength = 20000;
int minStepLength =200;
int steps =2 ;
unsigned long times = 0;
volatile int flag=1;
volatile int highFlag=0;
void setup() {
//Serial.begin(250000);
 /* pinMode(AA1,OUTPUT);
  pinMode(AA2,OUTPUT);
  pinMode(BB1,OUTPUT);
  pinMode(BB2,OUTPUT);
  pinMode(CC1,OUTPUT);
  pinMode(CC2,OUTPUT);*/
  pinMode(emfA,INPUT);
  pinMode(emfB,INPUT);
  pinMode(emfC,INPUT);
 // Wire.begin(7);
 // Wire.onReceive(receiveEvt);
AA1;AA2;BB1;BB2;CC1;CC2;
 //ADCSRA &= ~PS_128;
 //ADCSRA |=PS_8; 

 //ADCSRA |= PS_16;


ADCSRB  |= (1<<ACME);
  DIDR1 |= (1<<AIN0D);
  ACSR  |= (1<<ACIE);

  
}
ISR (ANALOG_COMP_vect) {
//if(rotor_run == 200) //next_commutate_state (0);
  /*
  rotor_run++;
  if(rotor_run > 200)
  {
    rotor_run = 200;
  }*/
  //Serial.println("1");
  }
/* void receiveEvt(int bytes){
     if(flag==1){  

    minStepLength=Wire.read(); 
    flag=0; 
  
  }
 }
*/
void wait(unsigned long i){
  flag = 1 ;

  if (i > 16380) {
  times=millis()+i/1000;
  while(millis()<times){
    
    }
  }
  else{
   times=micros()+i;
      while(micros()<times){
    
    }
    
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
     // delta = emA-sum;
     SENSE_U;
      break;

  //Phase2 A-B
    case 2:
      CC1_OFF;
     // AA2_OFF;
     // BB1_OFF;
     // CC2_OFF;
      if(highFlag==0){AA1_ON;highFlag==1;}
      else{AA1_OFF;highFlag==0;}
      BB2_ON;
      //myDelay(stepLength);
       //delta = emC-sum;
      SENSE_W;
      break;

  //Phase3 A-C
    case 3: 
       BB2_OFF;
      //CC1_OFF;
     // AA2_OFF;
     // BB1_OFF;
     if(highFlag==0){AA1_ON;highFlag==1;}
      else{AA1_OFF;highFlag==0;}
      CC2_ON;
      //delta = emB-sum;
      SENSE_V;
    break;   
  
  //Phase4 B-C
  case 4:
      AA1_OFF;
     // AA2_OFF;
      
      //BB2_OFF;
      //CC1_OFF;
       if(highFlag==0){BB1_ON;highFlag==1;}
      else{BB1_OFF;highFlag==0;}
      CC2_ON;
      SENSE_U;
      //myDelay(stepLength);
      //delta = emA-sum;
      break;

  //Phase5 B-A 
  case 5:
  //AA1_OFF;
      CC2_OFF;
     // BB2_OFF;
      //CC1_OFF;
      if(highFlag==0){BB1_ON;highFlag==1;}
      else{BB1_OFF;highFlag==0;}
      AA2_ON;
      SENSE_W;
      //myDelay(stepLength);
     // delta = emC-sum;
      break;

  //Phase6 C-A
  case 6:
   //AA1_OFF;
      
      BB1_OFF;
     // CC2_OFF;
      //BB2_OFF;
      if(highFlag==0){CC1_ON;highFlag==1;}
      else{CC1_OFF;highFlag==0;}
      AA2_ON;
      SENSE_V;
      //myDelay(stepLength);
       delta = emB-sum;
  break;
  }  
   



}








void loop() {
//emA = analogRead(emfA);
  //      emB = analogRead(emfB);
    //    emC = analogRead(emfC);
        sum = (emA+emB+emC)/3;
 //Serial.print(emA);Serial.print("  ");Serial.print(emB);Serial.print("  ");Serial.print(emC);Serial.print("  ");Serial.print(+delta);Serial.println("");


 // SENSE_V;
  //SENSE_H;
 
  //Serial.println("0");

  //!SENSE_H;
if(stepLength<400 ){

  if(SENSE_H){
    
    fase+=1;
     if (fase > 6) {
          fase = 1;
          }
    
    }

 if(!SENSE_H){
    
    fase+=1;
     if (fase > 6) {
          fase = 1;
          }
    
    }



/*
  if (Lastdelta < 0){
  if (delta > 0)
      {
        Lastdelta=delta; //save the last delta
        fase= fase + 1;
        if (fase > 6) {
          fase = 1;
          }
      }
  }//Zero cross from - to +  

   if (Lastdelta > 0){
  if (delta < 0)
      {
        Lastdelta=delta;
        fase= fase + 1;
        if (fase > 6) {
          fase = 1;
          }
      }
  }
  */

}

else{
  
    if (fase<6){
    fase=fase+1;}
  else{
    fase=1;
  }
  
  
  
  }


  
  

  wait(stepLength);
  //wait(1000);
  //myDelay(stepLength);
 // switchStep(2); 
 // switchStep(3);
 // switchStep(4);
  //switchStep(5);
 // switchStep(6);
  switchStep(fase);
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

  else if (stepLength < 2000) {
   // digitalWrite(led, HIGH);      // fourth gear
    steps =1;
  }
}







