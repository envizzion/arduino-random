
#include <stdint.h>  
#include <avr/io.h>
#include <util/twi.h>
#include <stdint.h>




// Define various ADC prescaler
const unsigned char PS_8 = (1 << ADPS1)| (1 << ADPS0);
const unsigned char PS_16 = (1 << ADPS2);
//const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
//const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
//const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

//volatile int emfA=A2;
//volatile  int emfB=A3;
//volatile int emfC=A4;

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

 int delta= 0;
 int Lastdelta= -1;

unsigned long stepLength = 20000;
int minStepLength = 2;
int steps =2 ;

 int emA=0;
 int emB=0;
 int emC=0;
 int sum=0;
 int fase=1;




void setup() {
//Serial.begin(250000);


  ADMUX = (1<<REFS0);
  ADCSRA &= ~(1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
 // ADCSRA |=PS_8;
 // ADCSRA |=  (1 << ADPS2);
 //ADCSRA |=  (1 << ADPS1);

 //ADCSRA &= ~PS_128;

// ADCSRA |= PS_16;
}  
  


void loop() {
        emA = adc_read(0);
        emB = adc_read(1);
        emC = adc_read(2);
        sum = (emA+emB+emC)/3;
// Serial.print(emA);Serial.print("  ");Serial.print(emB);Serial.print("  ");Serial.print(emC);Serial.print("  ");Serial.print(Lastdelta);Serial.println("");

if(stepLength<1000){



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

}

else{
  
    if (fase<6){
    fase=fase+1;}
  else{
    fase=1;
  }
  
  
  
  }


  
  switchStep(fase);
  
  myDelay(stepLength);
 // switchStep(2); 
 // switchStep(3);
 // switchStep(4);
  //switchStep(5);
 // switchStep(6);
  
  if(stepLength > minStepLength)
  {
    stepLength = stepLength - steps;
  } else {
    // set the minimum pulse length
    stepLength=minStepLength;
  }
  
  if (stepLength < 39950) {
    //digitalWrite(led, HIGH);   // second gear
    steps = 500;
  }
   
  if (stepLength < 20000) {
//digitalWrite(led, LOW);      // third gear
    steps = 50;
  }

  if (stepLength < 2000) {
   // digitalWrite(led, HIGH);      // fourth gear
    steps =1;
  }
}


void switchStep(int stage)
{
  switch(stage)
  {





    //Phase1 C-B
 
    case 1:
      AA1_OFF;
      AA2_OFF;
      BB1_OFF;
      CC2_OFF;
      CC1_ON;
      BB2_ON;
      
      //myDelay(stepLength);
      delta = emA-sum;
      break;

  //Phase2 A-B
    case 2:
      CC1_OFF;
      AA2_OFF;
      BB1_OFF;
      CC2_OFF;
      AA1_ON;
      BB2_ON;
      
      //myDelay(stepLength);
       delta = emC-sum;
      break;

  //Phase3 A-C
    case 3: 
      BB2_OFF;
      CC1_OFF;
      AA2_OFF;
      BB1_OFF;
      AA1_ON;
      CC2_ON;
      
      //myDelay(stepLength);
      delta = emB-sum;
    break;   
  
  //Phase4 B-C
  case 4:
       AA1_OFF;
      AA2_OFF;
      
      BB2_OFF;
      CC1_OFF;
      BB1_ON;
      CC2_ON;
      //myDelay(stepLength);
      delta = emA-sum;
      break;

  //Phase5 B-A 
  case 5:
       AA1_OFF;
      CC2_OFF;
      BB2_OFF;
      CC1_OFF;
      BB1_ON;
      AA2_ON;
      
      //myDelay(stepLength);
      delta = emC-sum;
      break;

  //Phase6 C-A
  case 6:
      AA1_OFF;
      
      BB1_OFF;
      CC2_OFF;
      BB2_OFF;
      CC1_ON;
      AA2_ON;
      //myDelay(stepLength);
       delta = emB-sum;
  break;
  }  
   



}


void myDelay(unsigned long p) {
if (p > 16380) {
  delay (p/1000);
  } else {
  delayMicroseconds(p);
  }
} 



// this code scans ADC1 for an analog signal upon request, using 8Mhz processor clock

     // needed for uint8_t



     
 int adc_read( int ch)
{
  
  // select the corresponding channel 0~7
  // ANDing with ’7′ will always keep the value
  // of ‘ch’ between 0 and 7
    // ch &= 0b00000111;  // AND operation with 7
  ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
 //ADMUX=ch;
  // start single convertion
  // write ’1′ to ADSC
  ADCSRA |= (1<<ADSC);
 
  // wait for conversion to complete
  // ADSC becomes ’0′ again
  // till then, run loop continuously
  while(ADCSRA & (1<<ADSC));
 
  return (ADC);
}




































































































