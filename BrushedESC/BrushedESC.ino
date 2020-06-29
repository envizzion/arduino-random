//PHASE2 (V)
#define VH_DDR  DDRB |= (1<<3);
#define VH_ON  TCCR2A |= (1<<COM2A1);
#define VH_OFF TCCR2A &= ~(1<<COM2A1);

//PHASE2 (V)
#define VL_DDR  DDRB |= (1<<2);
#define VL_ON PORTB |= (1<<2);
#define VL_OFF  PORTB &= ~(1<<2);


//PHASE3 (W)
#define WH_DDR  DDRD |= (1<<3);
#define WH_ON TCCR2A |= (1<<COM2B1);
#define WH_OFF  TCCR2A &= ~(1<<COM2B1);

//PHASE3 (W)
#define WL_DDR  DDRB |= (1<<0);
#define WL_ON PORTB |= (1<<0);
#define WL_OFF  PORTB &= ~(1<<0);


#define PHASE_ALL_OFF VH_OFF;VL_OFF;WH_OFF;WL_OFF;

volatile float rx_pwm;
byte last_channel_1, last_channel_2, last_channel_3, last_channel_4;
volatile int receiver_input_channel_1, receiver_input_channel_2, receiver_input_channel_3, receiver_input_channel_4;
int counter_channel_1, counter_channel_2, counter_channel_3, counter_channel_4, start;
volatile float receiver_input[5];
//int temp;
unsigned long timer_channel_1, timer_channel_2, timer_channel_3, timer_channel_4, esc_timer, esc_loop_timer;
unsigned long zero_timer, timer_1, timer_2, timer_3, timer_4, current_time;

void setup() 
//############################################################################
{
  Serial.begin(9600);
  //Watchdog on
  //WDTCSR = (1<<WDCE) | (1<<WDE);
  
  //Motordriver output

  VH_DDR;
  WH_DDR;
  
  VL_DDR;
  WL_DDR;
    

   TCCR0A |= (1<<COM0B1|1<<WGM01|1<<WGM00);
  TCCR0B |= (1<<CS00);
  
  TCCR2A |= (1<<COM2A1|1<<COM2B1|1<<WGM21|1<<WGM20);
  TCCR2B |= (1<<CS20);
  


  PHASE_ALL_OFF;
  

    
  //I2C Init
 // TWAR = MOTOR_ADR & 0xFE;
   // TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);
 PCICR |= (1 << PCIE2);                                       //Set PCIE0 to enable PCMSK0 scan.
  PCMSK2 |= (1 << PCINT18); 
  //Interrupts enabel
  sei();
  VH_ON;
  WL_ON;
rx_pwm=0;
}


ISR(PCINT2_vect){
  current_time = micros();
  
  //Channel 1=========================================
  if(PIND & B00000100){                                        //Is input 8 high?
    if(last_channel_1 == 0){                                   //Input 8 changed from 0 to 1
      last_channel_1 = 1;                                      //Remember current input state
      timer_1 = current_time;                                  //Set timer_1 to current_time
    }
  }
  else if(last_channel_1 == 1){                                //Input 8 is not high and changed from 1 to 0
    last_channel_1 = 0;                                        //Remember current input state
    receiver_input[1] = current_time - timer_1;                 //Channel 1 is current_time - timer_1
    rx_pwm = ((receiver_input[1]-1000)/1000)*254;
  }
}

void loop(){
 // while(1)
  //{ 

 Serial.println( rx_pwm);     

        OCR2A = rx_pwm;
      OCR2B = rx_pwm;
        //OCR0B = rx_pwm;
       //rx_pwm++;
      // delay(200);
       //rx_pwm++;
       //delay(50);
      // if(rx_pwm==250){
        
       // rx_pwm=0;
    //    }
     //  }

//}

}

