








































































// Define various ADC prescaler
const unsigned char PS_8 = (1 << ADPS1)| (1 << ADPS0);
const unsigned char PS_16 = (1 << ADPS2);
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
#include<Wire.h>;







int emfA=A0;
int emfB=A1;
int emfC=A2;






static int delta= 0;
      static int Lastdelta= -1;
 int emA = 0;
      int emB =0;
      int emC =0;
      int sum =0;  
      int fase=1;


int AA1=13;
int AA2=12;
int BB1=11;
int BB2=10;
int CC1=9;
int CC2=8;
//int led = 6;
unsigned long stepLength = 20000;
 int minStepLength =1;
int steps =2;

void setup() {
//Serial.begin(250000);
  pinMode(AA1,OUTPUT);
  pinMode(AA2,OUTPUT);
  pinMode(BB1,OUTPUT);
  pinMode(BB2,OUTPUT);
  pinMode(CC1,OUTPUT);
  pinMode(CC2,OUTPUT);
  pinMode(emfA,INPUT);
  pinMode(emfB,INPUT);
  pinMode(emfC,INPUT);
//Wire.begin(7);
//Wire.onReceive(receiveEvt);
 ADCSRA &= ~PS_128;
 //ADCSRA |=PS_8; 

 ADCSRA |= PS_16;
  
}

void myDelay(unsigned long p) {
if (p > 16380) {
  delay (p/1000);
  } else {
  delayMicroseconds(p);
  }
} 

//void receiveEvt(int bytes){
 //   minStepLength=Wire.read();  
  
 // }



void loop() {
 //stepLength=analogRead(A4);

emA = analogRead(emfA);
//minStepLength = analogRead(A4);
        emB = analogRead(emfB);
        emC = analogRead(emfC);
        sum = (emA+emB+emC)/3;
//Serial.print(emA);Serial.print("  ");Serial.print(emB);Serial.print("  ");Serial.print(emC);Serial.print("  ");Serial.print(+delta);Serial.println("");

if(stepLength<500){



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
  }
//else if(stepLength < minStepLength){
//   stepLength=stepLength+steps;
//  }  
  else {
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
     digitalWrite(AA1,LOW);
      digitalWrite(AA2,LOW);
     digitalWrite(BB1,LOW);
      digitalWrite(CC2,LOW);
       digitalWrite(BB2,HIGH);
       digitalWrite(CC1,HIGH);
     
     
      //myDelay(stepLength);
      delta = emA-sum;
      break;

  //Phase2 A-B
    case 2:
      digitalWrite(AA2,LOW);
      digitalWrite(BB1,LOW);
      digitalWrite(CC1,LOW);
      digitalWrite(CC2,LOW);
       digitalWrite(BB2,HIGH);
      digitalWrite(AA1,HIGH);
     
      //myDelay(stepLength);
       delta = emC-sum;
      break;

  //Phase3 A-C
    case 3: 
      digitalWrite(AA2,LOW);
      digitalWrite(BB1,LOW);
      digitalWrite(BB2,LOW);
      digitalWrite(CC1,LOW);
      digitalWrite(AA1,HIGH);
      digitalWrite(CC2,HIGH);
      
      //myDelay(stepLength);
      delta = emB-sum;
    break;   
  
  //Phase4 B-C
  case 4:
      digitalWrite(AA1,LOW);
      digitalWrite(AA2,LOW);
     digitalWrite(BB2,LOW);
      digitalWrite(CC1,LOW);
       digitalWrite(CC2,HIGH);
      digitalWrite(BB1,HIGH);
     
      //myDelay(stepLength);
      delta = emA-sum;
      break;

  //Phase5 B-A 
  case 5:
      digitalWrite(AA1,LOW);
      digitalWrite(BB2,LOW);
      digitalWrite(CC1,LOW);
      digitalWrite(CC2,LOW);
      digitalWrite(BB1,HIGH);
      digitalWrite(AA2,HIGH);
      
      //myDelay(stepLength);
      delta = emC-sum;
      break;

  //Phase6 C-A
  case 6:
     digitalWrite(AA1,LOW);
      digitalWrite(BB1,LOW);
     digitalWrite(BB2,LOW);
      digitalWrite(CC2,LOW);
      digitalWrite(AA2,HIGH);
      digitalWrite(CC1,HIGH);
      
      //myDelay(stepLength);
       delta = emB-sum;
  break;
  }  
   



}



