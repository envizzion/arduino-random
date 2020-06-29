#define EN1  DDRB |= (1<<3);
#define EN1_ON  TCCR2A |= (1<<COM2A1);



#define IN1_ON  DDRB |= (1<<2);
#define IN1_OFF  DDRB &=~(1<<2);

#define IN2_ON  DDRB |= (1<<1);
#define IN2_OFF  DDRB &=~(1<<1);

#define IN3_ON  DDRB |= (1<<0);
#define IN3_OFF  DDRB &=~(1<<0);

#define IN4_ON  DDRD |= (1<<7);
#define IN4_OFF  DDRD &=~(1<<7);


#define EN2  DDRD |= (1<<3);
#define EN2_ON TCCR2A |= (1<<COM2B1);

#define All_OFF ;IN1_OFF;IN2_OFF;IN3_OFF;IN4_OFF;

#define FORWARD ; IN1_ON; IN2_OFF; IN3_ON; IN4_OFF;

#define REVERSE ;IN1_OFF;IN2_ON;IN3_OFF;IN4_ON;

#define LEFT;IN1_OFF;IN2_ON; IN3_ON; IN4_OFF;

#define RIGHT ; IN1_ON; IN2_OFF;IN3_OFF;IN4_ON;


void setup() {


//adc_start(ADC_PRESCALER_16, ADC_VREF_AVCC, 7, myfunction);

  TCCR2A |= (1<<COM2A1|1<<COM2B1|1<<WGM21|1<<WGM20);
  TCCR2B |= (1<<CS20);

 // EN1_ON
 // EN2_ON
//  OCR2A = 200;
//OCR2B = 200;
  FORWARD;
  
  
  
  

}

void loop() {
  // put your main code here, to run repeatedly:


}
