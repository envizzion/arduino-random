//#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/twi.h>
volatile byte last_channel_1, last_channel_2, last_channel_3, last_channel_4;
byte eeprom_data[36];
volatile int receiver_input_channel_1, receiver_input_channel_2, receiver_input_channel_3, receiver_input_channel_4;
int counter_channel_1, counter_channel_2, counter_channel_3, counter_channel_4, start;
volatile int receiver_input[5];
//int temp;
volatile unsigned long timer_channel_1, timer_channel_2, timer_channel_3, timer_channel_4, esc_timer, esc_loop_timer;
volatile unsigned long zero_timer, timer_1, timer_2, timer_3, timer_4, current_time;
volatile unsigned long time1=0,tCounter=0,prevT=0;

//#define F_CPU 16000000UL
void setup() {
// TCCR0A |= (1<<COM0B1|1<<WGM01|1<<WGM00);
  //TCCR0B |= (1<<CS00);
    //TIMER1 for start commutation or Back EMF lost
    TCCR1A=0x00;
  TCCR1B =0b00000010;
 TIMSK1 |= 0b00000100;
  // put your setup code here, to run once:
 PCICR |= (1 << PCIE0);                             // set PCIE0 to enable PCMSK0 scan
  PCMSK0 |= (1 << PCINT0);                           // set PCINT0 (digital input 8) to trigger an interrupt on state change
 // PCMSK0 |= (1 << PCINT1);                           // set PCINT1 (digital input 9)to trigger an interrupt on state change
 // PCMSK0 |= (1 << PCINT2);                           // set PCINT2 (digital input 10)to trigger an interrupt on state change
 // PCMSK0 |= (1 << PCINT3);                           // set PCINT3 (digital input 11)to trigger an interrupt on state change
Serial.begin(250000);
sei();
}


ISR (TIMER1_OVF_vect)
//############################################################################
{  
//time1=millis();
//Serial.println("interrupted");
//prevT=millis();



}



ISR(PCINT0_vect){
  current_time = TCNT1/2;
  
  //Channel 1=========================================
  if(PINB & B00000001){                                        //Is input 8 high?
    if(last_channel_1 == 0){                                   //Input 8 changed from 0 to 1
      last_channel_1 = 1;                                      //Remember current input state
      timer_1 = current_time;                                  //Set timer_1 to current_time
    }
  }
  else if(last_channel_1 == 1){     
     if(current_time<timer_1) current_time =  current_time + 65535  ;
    //Input 8 is not high and changed from 1 to 0
    last_channel_1 = 0;                                        //Remember current input state
    receiver_input[1] = ((current_time - timer_1)-1000)/4;                 //Channel 1 is current_time - timer_1
 TCNT1=1;
 Serial.println(receiver_input[1]);
  }
}
void loop() {
  // put your main code here, to run repeatedly:
//Serial.print(TCNT1);Serial.print(" ");Serial.print(micros());Serial.println("");

//Serial.print(time1);Serial.print(" ");Serial.print(tCounter);Serial.println("");
//prevT=TCNT1;
//if(TCNT1>prevT){prevT=TCNT1;
//Serial.print(TCNT1L);Serial.print(" ");Serial.print(TCNT1H);Serial.print(" ");Serial.print(TCNT1);Serial.println("");}

//uint8_t tmp = TCNT1L;
//uint16_t result = (TCNT1H << 8) | tmp;




//Serial.println(TCNT1);

}

