#include <atmega-adc.h>
#include <L298N.h>
uint16_t rawVals[]={0,0,0,0,0,0,0,0};
uint16_t currVals[]={0,0,0,0,0,0,0,0};
uint16_t maxVal=0,minVal=0,avg=0,avgCounts=0;
long int avgSum=0;
byte a[]={0,0,0,0,0};
int pTmp;

//--pid--//
float kp=0.2 ,kd=5,ki=0,lastErr,Speed;
float rbSpeed=60;
float lbSpeed=60;
float mSpeed=0;
int rpwm,lpwm;
//--motor control--//

//RIGHT MOTOR
#define EN1  DDRB |= (1<<3);
#define EN1_ON  TCCR2A |= (1<<COM2A1);


#define IN1  DDRB |= (1<<5);
#define IN1_ON  PORTB |= (1<<5);
#define IN1_OFF  DDRB &=~(1<<5);

#define IN2  DDRB |= (1<<4);
#define IN2_ON  PORTB |= (1<<4);
#define IN2_OFF  PORTB &=~(1<<4);

//LEFT MOTOR
#define IN3  DDRB |= (1<<1);
#define IN3_ON  PORTB |= (1<<1);
#define IN3_OFF  PORTB &=~(1<<1);

#define IN4 DDRB |= (1<<0);
#define IN4_ON  PORTB |= (1<<0);
#define IN4_OFF  PORTB &=~(1<<0);


#define EN2  DDRB |= (1<<2);
#define EN2_ON TCCR1A |= (1<<COM1B1);

#define All_OFF ;IN1_OFF;IN2_OFF;IN3_OFF;IN4_OFF;

#define FORWARD ; IN1_ON; IN2_OFF; IN3_ON; IN4_OFF;

#define REVERSE ;IN1_OFF;IN2_ON;IN3_OFF;IN4_ON;

#define LEFT;IN1_OFF;IN2_ON; IN3_ON; IN4_OFF;

#define RIGHT ; IN1_ON; IN2_OFF;IN3_OFF;IN4_ON;

//int rpwm=60;
//int lpwm=60;


void setup() {
Serial.begin(9600);
IN1;IN2;IN3;IN4;EN1;EN2;
pinMode(3,OUTPUT);
//adc_start(ADC_PRESCALER_64, ADC_VREF_AVCC, 7, myfunction);

TCCR1A=0x00;
TCCR1B=0x00;

TCCR2A=0x00;
TCCR2B=0x00;

TCCR2A |= (1<<COM2A1|1<<COM2B1|1<<WGM21|1<<WGM20);
TCCR2B |= (1<<CS20);

  TCCR1A |=(1<<COM1B1|1<<WGM10);
  TCCR1B |= (1<<CS10|1<<WGM12);

 // EN1_ON;
 // EN2_ON;
  OCR2A =rpwm;
  OCR1B =lpwm;
  //PORTB &=~(1<<2);
  //PORTB &=~(1<<3);
  while(1)if(digitalRead(A7)==HIGH)break;

}

void myfunction(uint8_t pin, uint16_t value) {
  switch(pin) {
    case(0):
      rawVals[0]=value;
      break;
      
    case(1):
      rawVals[1]=value;
      
      break;
      
    case(2):
       rawVals[2]=value;
      
      break;
    case(3):
       rawVals[3]=value;
      
      break;
    case(4):
       rawVals[4]=value;
      
      break;
    case(5):
       rawVals[5]=value;
      
      break;
    case(6):
      rawVals[6]=value;
      
      break;
    case(7):
       rawVals[7]=value;
      
      break;
   
      
     
  }
}

/*
void wait(unsigned long i){
  flag = 1 ;

  if (i > 16380) {
  times=millis()+i/1000;
  while(millis()<times){
    
    }
  }
  else{
   times=micros()+i;
      while(micros()<times){
    
    }
    
    }
}
*/
  
//----IR CONTROL---//

//trackFinder
byte trF(){
    //black=1
    //white=0
    byte x;
    if(currVals[0]<avg)a[0]=1;
    if(currVals[1]<avg)a[1]=1;
    if(currVals[2]<avg)a[2]=1;
    if(currVals[3]<avg)a[3]=1;
    if(currVals[4]<avg)a[4]=1;

    if(a[0]==0 &&  a[2]==1 && a[4]==0 )x=1;
      if(a[0]==1 &&  a[2]==0 && a[4]==1 )x=0;
    
    
    for(int i=0;i<5;i++){a[i]=0;}
    return x;
   
  }

int PanelPos(){

    byte p=trF(); 
    
    if(currVals[0]>avg)a[0]=1;
    if(currVals[1]>avg)a[1]=1;
    if(currVals[2]>avg)a[2]=1;
    if(currVals[3]>avg)a[3]=1;
    if(currVals[4]>avg)a[4]=1;

    
    if(a[0]==p && a[1]==p && a[2]==p && a[3]==p && a[4]==p )pTmp=11111;
    if(a[0]!=p && a[1]!=p && a[2]!=p && a[3]!=p && a[4]!=p )pTmp=22222;
    if(a[0]!=p && a[1]!=p && a[2]==p && a[3]!=p && a[4]!=p )pTmp=22222;
    
      
    
 
  
  return 1;
  }




//---------//


//----MOTOR CONTROL----//

void drive(byte x){
  switch(x){
   case 1: FORWARD;
   break;
   case 2: REVERSE;
   break;
   case 3: LEFT;
   break;
   case 4:RIGHT; 
   break;


       
    
    }
    
    } 
  
//-----PID----//  


float error(){
  
   byte p=trF(); 
   float err; 
    if(currVals[0]>avg)a[0]=1;
    if(currVals[1]>avg)a[1]=1;
    if(currVals[2]>avg)a[2]=1;
    if(currVals[3]>avg)a[3]=1;
    if(currVals[4]>avg)a[4]=1;

    
    if(a[1]==p && a[2]==p && a[3]==p)  err=0 ; //111
    if(a[1]==p && a[2]==p && a[3]!=p ) err=500;  //110
    if(a[1]!=p && a[2]==p && a[3]==p ) err=-500;   //011
      if(a[1]==p && a[2]!=p && a[3]!=p ) err=1000;  //100
    if(a[1]!=p && a[2]!=p && a[3]==p ) err=-1000 ;  //001
                                                    
  return err;
  }

void pidFollow(){
 
  
  mSpeed= kp*error() + kd*(error()-lastErr);
  float lastErr=error();
  
  rpwm=rbSpeed +mSpeed; 
  lpwm=lbSpeed-mSpeed;
  }


void calliberate(){
  for(int i=0;i<8;i++){
   if(maxVal<=currVals[i])maxVal=currVals[i];
   if(minVal>currVals[i])minVal=currVals[i];
  }  
  
  avg=(maxVal+minVal)/2;
  
  }

void loop() {
  // put your main code here, to run repeatedly:
/*
//---calliberation---//
while(millis()>3000){
 avgCounts++;
for(int i=0;i<8;i++){
  currVals[i]=rawVals[i];
  }

calliberate();
avgSum+=avg;
if(avgCounts==1000)
{avg=avgSum/1000;
 
}  
}  */
//-------//
/*for(int i=0;i<8;i++){
  currVals[i]=rawVals[i];
  }
*/
IN1_ON;
IN2_OFF;
IN3_ON;
IN4_OFF;
  OCR2A = 100;
   //OCR1B=100;
  while(lpwm<255){
    delay(10);
    lpwm+=1;
     OCR1B=lpwm;
     OCR2A = lpwm;
    // OCR2B=lpwm;
    
    // OCR2A=100;
    if(lpwm==250)lpwm=0;
    } 
 
  

//EN1_ON;
//EN2_ON;
//REVERSE;



/*
for(int i=0;i<8;i++){
Serial.print(currVals[i]);
Serial.print(" ");

}
Serial.print(A7);

Serial.println("");

*/
}
