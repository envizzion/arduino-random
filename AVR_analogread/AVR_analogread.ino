#include <avr/io.h>
#include <avr/interrupt.h>
int x;
int main(void)
{Serial.begin(9600);

ADCSRA |= 1<<ADPS2;
ADMUX |= 1<<REFS0 ;
ADCSRA |= 1<<ADIE;
ADCSRA |= 1<<ADEN;
ADMUX=0xD0;
sei();

ADCSRA |= 1<<ADSC;

while (1)
{
  Serial.println(x);
  
}
}
ISR(ADC_vect)
{
uint8_t theLow = ADCL;
uint16_t theTenBitResult = ADCH<<8 | theLow;

switch (ADMUX)
{
case 0:

x=theTenBitResult;
break;
case 0xC1:

ADMUX = 0xD0;
break;
default:
//Default code
break;
} ADCSRA |= 1<<ADSC; 
}
