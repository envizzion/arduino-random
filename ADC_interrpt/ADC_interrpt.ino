

void adc_init()
{
    // AREF = AVcc
    ADMUX = (1<<REFS0);
 
    // ADC Enable and prescaler of 128
    // 16000000/128 = 125000
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}


int adc_read(int ch)
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



void setup(){
  Serial.begin(9600);
  adc_init();
  }


  void loop(){
    //int x=adc_read(0);
   // Serial.print(analogRead(A0));
    //Serial.print(" ");
   // Serial.print(analogRead(A1));
   // Serial.println();
   // delay(300);
    int y=adc_read(0);
    int x=adc_read(1);
   Serial.print(x);Serial.print(" ");Serial.print(y);Serial.println();
    
    
    }






 
