#include <avr/io.h>
#include <avr/interrupt.h>
  
// global variable to count the number of overflows
volatile uint8_t tot_overflow;
volatile int flag=0;
// initialize timer, interrupt and variable
void timer1_init()
{
    // set up timer with prescaler = 8
    TCCR1B |= (1 << CS11);
  
    // initialize counter
    TCNT1 = 0;
  
    // enable overflow interrupt
    TIMSK1 |= (1 << TOIE1);
  
    // enable global interrupts
    sei();
  
    // initialize overflow counter variable
    tot_overflow = 0;
}
  
// TIMER1 overflow interrupt service routine
// called whenever TCNT1 overflows
ISR(TIMER1_OVF_vect)
{
    // keep a track of number of overflows
 //   tot_overflow++;
    
    // check for number of overflows here itself
    // 61 overflows = 2 seconds delay (approx.)
  //  if (tot_overflow >= 61) // NOTE: '>=' used instead of '=='
  //  {
       flag=1;  // toggles the led
        // no timer reset required here as the timer
        // is reset every time it overflows
  
      //  tot_overflow = 0;   // reset overflow counter
   // }
}
  
void setup(){

  Serial.begin(250000);
    // connect led to pin PC0
    DDRC |= (1 << 0);
  
    // initialize timer
    timer1_init();
  
    // loop forever
} 
    
void    loop()
    {
    if (flag==1){Serial.println(millis());flag=0;}
    }

