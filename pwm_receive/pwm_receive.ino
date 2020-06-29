

#include <atmega-adc.h>
volatile byte last_channel_1, last_channel_2, last_channel_3, last_channel_4;


unsigned long timer_channel_1, timer_channel_2, timer_channel_3, timer_channel_4, esc_timer, esc_loop_timer;
volatile unsigned long timer_1,times;//, timer_2, timer_3, timer_4, current_time;
unsigned long loop_timer;
int receiver_input[5];
volatile unsigned long tmpPWM;
volatile int maxPWM=1500;
volatile int minPWM=1500;
unsigned long mintime,maxtime,ttime=1000000000;
volatile unsigned long minStepLength;
volatile unsigned long current_time;
int a1,a2,a3;
//int tmpMaxPWM=0;
void setup() {
 

  // put your setup code here, to run once:
PCICR |= (1 << PCIE2);                                       //Set PCIE0 to enable PCMSK0 scan.
  PCMSK2 |= (1 << PCINT23);
  
  Serial.begin(9600);
  //DDRD|= (1 << 7);
  ///pinMode(7,INPUT); 
  while(millis()<3000){
    current_time = micros();

  //Channel 1=========================================
 
  if(PIND & B10000000){                                        //Is input 8 high?
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
  if(PIND & B10000000){                                        //Is input 8 high?
    if(last_channel_1 == 0){                                   //Input 8 changed from 0 to 1
      last_channel_1 = 1;                                      //Remember current input state
      timer_1 = current_time;                                  //Set timer_1 to current_time
    }
  }
  else if(last_channel_1 == 1){                                //Input 8 is not high and changed from 1 to 0
    last_channel_1 = 0;                                        //Remember current input state
    tmpPWM= current_time - timer_1;                //Channel 1 is current_time - timer_1
    minStepLength=1+maxPWM-tmpPWM;              //Channel 1 is current_time - timer_1
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
void loop() {
  // put your main code here, to run repeatedly:
mintime=micros();
Serial.print(minStepLength);
//wait(4000);
/*
maxtime=micros();

//if((maxtime-mintime)<=ttime){
  ttime=maxtime-mintime;

Serial.print("  ");
//Serial.print(receiver_input[1]);

//}*/
Serial.println("");
}

