 #include <avr/io.h>
 #include <avr/interrupt.h>
  
  void waitForCallib(){
   pinMode(A1,INPUT);// DDRC|=(0<<1);
    while(1){if(digitalRead(A1)==HIGH)break;}
   Serial.println("recev start");
     DDRC|=(1<<1);
    }

volatile byte last_channel_1, last_channel_2;
volatile unsigned long timer_1, timer_2,current_time;
volatile int r1,r2,count;



void setup() {
  // put your setupcode here, to run once:
 Serial.begin(9600);
 PCICR |= (1 << PCIE1);                                       //Set PCIE0 to enable PCMSK0 scan.
  PCMSK1 |= (1 << PCINT8);
  sei();

  DDRD |=(1<<5); 
 // pinMode(1,OUTPUT);
 // PORTD|=(0<<1);
 // digitalWrite(1,LOW);
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
    
  }
}
void loop() {
Serial.print(r1);Serial.print("  ");Serial.println(count);

if(r1<=2100 && r1>=1900){
  PORTD |=(1<<5);
  }
else {
  PORTD &=~(1<<5);
  //digitalWrite(5,LOW);
}
}    
