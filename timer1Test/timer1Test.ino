#include<avr/io.h>
#include<avr/interrupt.h>

#define LED PB3
volatile unsigned long tcount=0,prevT=0;

ISR (TIMER1_OVF_vect)    // Timer1 ISR
{
//  PORTB ^= (1 << 4);  
  //TCNT1 = 63974;   // for 1 sec at 16 MHz
//   tcount=millis()-prevT;
 //  prevT=millis();
}

void setup()
{
  DDRB |= (1<<4);    //Configure the PORTD4 as output
  Serial.begin(250000);
  TCNT1 = 0;   // for 1 sec at 16 MHz 

  TCCR1A = 0x00;
  TCCR1B = (1<<CS11);;  // Timer mode with 1024 prescler
  //TIMSK1 = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
  sei();        // Enable global interrupts by setting global interrupt enable bit in SREG
  
  while(1){
  if(TCNT1>64000){
     tcount=millis()-prevT;
   prevT=millis();
  Serial.println(tcount); 

  }
  }
}


