#include <atmega-adc.h>


#define SENSE_U    ADMUX = 3
#define SENSE_V   ADMUX = 1
#define SENSE_W   ADMUX = 2
const unsigned char PS_16 = (1 << ADPS2);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
 int a1,a2,a3,flag;

void setup() {
  // put your setup code here, to run once:
 // adcInit();
Serial.begin(9600);

a1=0;a2=0;a3=0;
adc_start(ADC_PRESCALER_16, ADC_VREF_AVCC, 3, myfunction);
  
  // enable global interrupts
  sei();
flag=0;
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
      flag=1;
      break;
  }
}





void loop() {
  // put your main code here, to run repeatedly:
//Serial.println(adcRead(3));
//Serial.println(analogRead(A4));

if (flag==1){
Serial.print(a1);   Serial.print("  "); Serial.print(a2);  Serial.print("  "); Serial.print(a3);  Serial.println(""); 
  flag==0;
  }
//delay(1000);  
}

void adcInit(){
  
  ADMUX = (1<<REFS0);
   ADCSRA &= ~PS_128;
 //ADCSRA |=PS_8; 

 ADCSRA |= PS_16;
  
  }
uint16_t adcRead(uint8_t ch){
  ch &=0b00000111;
  ADMUX = (ADMUX & 0xF8)|ch;

  ADCSRA|=(ADSC);
  while(ADCSRA & (1<<ADSC));
    return (ADC);
    
    
  
  }



  
 
