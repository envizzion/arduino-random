
volatile byte last_channel_1, last_channel_2;
volatile unsigned long timer_1, timer_2,current_time;
volatile int r1,r2;
float kp,kd;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(A0,INPUT);
   pinMode(A5,INPUT);
  PCICR |= (1 << PCIE1);                                       //Set PCIE0 to enable PCMSK0 scan.
  PCMSK1 |= (1 << PCINT9); 
  PCMSK1 |= (1 << PCINT13); 
sei();
}



void setPID(int value,int variable){
  float val=value;
  float var=variable;
 // if(variable<550 && variable >450)
 kp=(val-1000)/1000;
  //if(variable<950 && variable >1050)
  kd=(var-1000)/1000;
  
  }




ISR(PCINT1_vect){
  current_time = micros();

  //Channel 1=========================================
  if(PINC & B00000010){                                        //Is input 8 high?
    if(last_channel_1 == 0){                                   //Input 8 changed from 0 to 1
      last_channel_1 = 1;                                      //Remember current input state
      timer_1 = current_time;                                  //Set timer_1 to current_time
    }
  }
  else if(last_channel_1 == 1){                                //Input 8 is not high and changed from 1 to 0
    last_channel_1 = 0;                                        //Remember current input state
    r1 = current_time - timer_1;                //Channel 1 is current_time - timer_1
    
  }
  //Channel 2=========================================
  if(PINC & B00100000 ){                                       //Is input 9 high?
    if(last_channel_2 == 0){                                   //Input 9 changed from 0 to 1
      last_channel_2 = 1;                                      //Remember current input state
      timer_2 = current_time;                                  //Set timer_2 to current_time
    }
  }
  else if(last_channel_2 == 1){                                //Input 9 is not high and changed from 1 to 0
    last_channel_2 = 0;                                        //Remember current input state
    r2 = current_time - timer_2;                //Channel 2 is current_time - timer_2
  
  }
 
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.print("kp= ");
Serial.print(r1);
Serial.print("  kd=");
Serial.println(r2);
//setPID(r2,r1); 
}
