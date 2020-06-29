 #include <avr/io.h>
 #include <avr/interrupt.h>
  void waitForCallib(){
   pinMode(A1,INPUT);// DDRC|=(0<<1);
    while(1){if(digitalRead(A1)==HIGH)break;}
   Serial.println("recev start");
     DDRC|=(1<<1);
    }

volatile byte last_channel_1, last_channel_2;
volatile unsigned long timer_1, timer_2,current_time,times;
volatile int r1,r2;

#include <Servo.h>
Servo ser;
void wait(unsigned long i){
 

 // if (i > 16380) {
 // times=millis()+i/1000;
 // while(millis()<times){
    
 //   }
  //}
 // else{
   times=micros()+i;
      while(micros()<times){
   // Serial.println(micros());
    }
}



void startCom(){
    
  pinMode(A1,OUTPUT);//A1 transmit signals
  pinMode(A5,INPUT);//A5 reads receiver

  PCICR |= (1 << PCIE1);                                       //Set PCIE0 to enable PCMSK0 scan.
  PCMSK1 |= (1 << PCINT13);   
  
  }
 void stopCom(){
  PCICR |= (0 << PCIE1);                                       //Set PCIE0 to enable PCMSK0 scan.
  PCMSK1 |= (0 << PCINT13);
  pinMode(A5,OUTPUT);

  }

  void blinkTestComm(){
    r1=0;
    startCom();
   while(r1<=1900 || r1>=2100){
    unsigned long tim=millis()+2000;
  while((tim-1000)>=millis()){    
  PORTC |=(1<<1);
  wait(1500);
  PORTC &=~(1<<1);
  wait(1500);
  }
    while((tim)>=millis()){    
  PORTC |=(1<<1);
  wait(2000);
  PORTC &=~(1<<1);
  wait(2000);
  }
   stopCom();
   
    }
  }

void responseTest(){
  startCom();
   
   Serial.println(r1);
  
  
  
}
    



void setup() {
  // put your setupcode here, to run once:
  Serial.begin(9600);

sei();




//blinkTestComm();
}

ISR(PCINT1_vect){
  current_time = micros();

  //Channel 1=========================================
  if(PINC & B00010000){                                        //Is input 8 high?
    if(last_channel_1 == 0){                                   //Input 8 changed from 0 to 1
      last_channel_1 = 1;                                      //Remember current input state
      timer_1 = current_time;                                  //Set timer_1 to current_time
    }
  }
  else if(last_channel_1 == 1){                                //Input 8 is not high and changed from 1 to 0
    last_channel_1 = 0;                                        //Remember current input state
    r1 = current_time - timer_1;                //Channel 1 is current_time - timer_1
    
  }
}
void loop() {
responseTest();
}   
