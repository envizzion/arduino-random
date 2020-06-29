
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/twi.h>


#define CC1  DDRB |= (1<<1);
#define CC1_ON   PORTB |= (1<<1);
#define CC1_OFF  PORTB &= ~(1<<1);

#define CC2  DDRB |= (1<<0);
#define CC2_ON   PORTB |= (1<<0);
#define CC2_OFF  PORTB &= ~(1<<0);
void setup() {
  // put your setup code here, to run once:
 CC1;
}

void loop() {
  // put your main code here, to run repeatedly:
CC1_ON;
//digitalWrite(8,HIGH);
delay(500);
CC1_OFF;
//digitalWrite(8,LOW);
delay(500);
}
