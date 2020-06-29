







  
















































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


#define SENSE_U    ADMUX = 0;
#define SENSE_V   ADMUX = 1 ;
#define SENSE_W   ADMUX = 2 ;

#define SENSE_H   (ACSR&(1<<ACO))













// Define various ADC prescaler
const unsigned char PS_8 = (1 << ADPS1)| (1 << ADPS0);
const unsigned char PS_16 = (1 << ADPS2);
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);



volatile unsigned char rotor_state = 0;




int emfA=A1;
int emfB=A2;
int emfC=A3;






static int delta= 0;
      static int Lastdelta= -1;
 int emA = 0;
      int emB =0;
      int emC =0;
      int sum =0;  
      int fase=1;


/*int AA1=13;
int AA2=12;
int BB1=11;
int BB2=10;
int CC1=9;
int CC2=8;*/
int led = 12;
unsigned long stepLength = 20000;
int minStepLength = 1;
int steps =2 ;

void setup() {
//Serial.begin(250000);
 /* pinMode(AA1,OUTPUT);
  pinMode(AA2,OUTPUT);
  pinMode(BB1,OUTPUT);
  pinMode(BB2,OUTPUT);
  pinMode(CC1,OUTPUT);
  pinMode(CC2,OUTPUT);*/
 // pinMode(emfA,INPUT);
//  pinMode(emfB,INPUT);
  //pinMode(emfC,INPUT);

 //ADCSRA &= ~PS_128;
 //ADCSRA |=PS_8; 

 //ADCSRA |= PS_16;


//Comperator init for back EMF
  ADCSRB  |= (1<<ACME);
  DIDR1 |= (1<<AIN0D);
  ACSR  |= (1<<ACIE);

  //Interrupts enabel
  sei();
}

void myDelay(unsigned long p) {
if (p > 16380) {
  delay (p/1000);
  } else {
  delayMicroseconds(p);
  }
} 




void loop() {
//emA = analogRead(emfA);
   //     emB = analogRead(emfB);
    //    emC = analogRead(emfC);
     //   sum = (emA+emB+emC)/3;
 //Serial.print(emA);Serial.print("  ");Serial.print(emB);Serial.print("  ");Serial.print(emC);Serial.print("  ");Serial.print(+delta);Serial.println("");

/*if(stepLength<500){



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


  
  switchStep(fase);*/

  

 if(stepLength>300){
 
 
 next_commutate_state(1);
 }
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


/*void switchStep(int stage)
{
  switch(stage)
  {





    //Phase1 C-B
 
    case 1:
     //AA1_OFF;
      AA2_OFF;
     // BB1_OFF;
      //CC2_OFF;
      CC1_ON;
      BB2_ON;
      //myDelay(stepLength);
      delta = emA-sum;
      break;

  //Phase2 A-B
    case 2:
      CC1_OFF;
     // AA2_OFF;
     // BB1_OFF;
     // CC2_OFF;
      AA1_ON;
      BB2_ON;
      //myDelay(stepLength);
       delta = emC-sum;
      break;

  //Phase3 A-C
    case 3: 
       BB2_OFF;
      //CC1_OFF;
     // AA2_OFF;
     // BB1_OFF;
      AA1_ON;
      CC2_ON;
      delta = emB-sum;
    break;   
  
  //Phase4 B-C
  case 4:
      AA1_OFF;
     // AA2_OFF;
      
      //BB2_OFF;
      //CC1_OFF;
      BB1_ON;
      CC2_ON;
      //myDelay(stepLength);
      delta = emA-sum;
      break;

  //Phase5 B-A 
  case 5:
  //AA1_OFF;
      CC2_OFF;
     // BB2_OFF;
      //CC1_OFF;
      BB1_ON;
      AA2_ON;
      //myDelay(stepLength);
      delta = emC-sum;
      break;

  //Phase6 C-A
  case 6:
   //AA1_OFF;
      
      BB1_OFF;
     // CC2_OFF;
      //BB2_OFF;
      CC1_ON;
      AA2_ON;
      //myDelay(stepLength);
       delta = emB-sum;
  break;
  }  
   



}
*/

ISR (ANALOG_COMP_vect) 
//############################################################################
{
  if(stepLength<300) next_commutate_state (0);
  
 
  
}







void next_commutate_state (unsigned char startup)
//############################################################################
{
  switch (rotor_state)
  {
    case (0):
      if(!SENSE_H || startup)
      {
       CC1_OFF;
     // AA2_OFF;
     // BB1_OFF;
     // CC2_OFF;
      AA1_ON;
   
        SENSE_W;
        rotor_state = 1;
       // TCNT1 = 1;
      }
      break;

    case (1):
      if(SENSE_H || startup)
      {
       BB2_OFF;
      //CC1_OFF;
     // AA2_OFF;
     // BB1_OFF;
      AA1_ON;
     
        SENSE_V;
        rotor_state = 2;
       // TCNT1 = 1;
      }
      break;

    case (2):
      if(!SENSE_H || startup)
      {
       AA1_OFF;
     // AA2_OFF;
      
      //BB2_OFF;
      //CC1_OFF;
      BB1_ON;
      
        SENSE_U;
        rotor_state = 3;
      //  TCNT1 = 1;
      }
      break;
  
    case (3):
      if(SENSE_H || startup)
      {
        CC2_OFF;
     // BB2_OFF;
      //CC1_OFF;
     
      AA2_ON;
        SENSE_W;
        rotor_state = 4;
       // TCNT1 = 1;
      }
      break;

    case (4):
      if(!SENSE_H || startup)
      {
       BB1_OFF;
     // CC2_OFF;
      //BB2_OFF;
      CC1_ON;
      
        SENSE_V;
        rotor_state = 5;
       // TCNT1 = 1;
      }
      break;

    case (5):
      if(SENSE_H || startup)
      {
        AA2_OFF;
     // BB1_OFF;
      //CC2_OFF;
      
      BB2_ON;
        SENSE_U;
        rotor_state = 0;
       // TCNT1 = 1;
      }
      break;
  }
}


