
volatile unsigned long current_time,timer_1;
volatile unsigned long rx_pwm,tmp_pwm=0;
volatile byte last_channel_1;
volatile int counter=0;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  PCICR |= (1 << PCIE2);                                       //Set PCIE0 to enable PCMSK0 scan.
  PCMSK2 |= (1 << PCINT18); 
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
   // tmpPWM= current_time - timer_1;                //Channel 1 is current_time - timer_1
    tmp_pwm+=current_time-timer_1;              //Channel 1 is current_time - timer_1
    counter++;
    if(counter==5){
      tmp_pwm=tmp_pwm/10;
      if(tmp_pwm<rx_pwm-4 || tmp_pwm>rx_pwm+4){rx_pwm=tmp_pwm;}
      tmp_pwm=0;
      counter=0;
      }
   
  }
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(rx_pwm);
}
