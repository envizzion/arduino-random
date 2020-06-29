#include <avr/io.h>
#include <avr/interrupt.h>

//========VARIABLES===============================
byte inMaze=1;
int turnSpeed=180;
byte LFollow=1;  //what side to follow in maze
byte LCount=0;  //no of checks for a left junc
byte RCount=0;  //no of checks for a right junc
byte midCount;
byte maxCount=25; 
//-----comparator-------//
unsigned long times=0;
volatile int alarm = 0;

byte controlPins[] = {B00000000, 
                  B00000100,
                  B00001000,
                  B00001100,
                  B00010000,
                  B00010100,
                  B00011000,
                  B00011100,
                  B00100000,
                  B00100100,
                  B00101000,
                  B00101100,
                  B00110000,
                  B00110100,
                  B00111000,
                  B00111100 }; 
 
// holds incoming values from 74HC4067                  
byte muxValues[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};

//---------------------//

//-----pid------
int errArr[]={0,0,0,0,0};
int error=0,lastErr=0,errCount=0;
float kp=3.45 ,kd=83,ki=0;

//--------------

//-----Communication------
volatile byte last_channel_1, last_channel_2;
volatile unsigned long timer_1, timer_2,current_time;
volatile int r1,r2;
//------------------------

//-------motorControl------//
int rbSpeed=240,rMax=255;
int lbSpeed=240,lMax=255;
int mSpeed=240;
int rpwm,lpwm;
//--motor control--//

//RIGHT MOTOR
#define EN1  DDRB |= (1<<3);
#define EN1_ON  TCCR2A |= (1<<COM2A1);


#define IN1  DDRB |= (1<<5);
#define IN1_ON  PORTB |= (1<<5);
#define IN1_OFF  PORTB &=~(1<<5);

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

#define ALL_OFF ;IN1_OFF;IN2_OFF;IN3_OFF;IN4_OFF;

#define FORWD ; IN1_ON; IN2_OFF; IN3_ON; IN4_OFF;

#define REVS ;IN1_OFF;IN2_ON;IN3_OFF;IN4_ON;

#define RIGHT;IN1_OFF;IN2_ON; IN3_ON; IN4_OFF;

#define LEFT ; IN1_ON; IN2_OFF;IN3_OFF;IN4_ON;

#define BRAKE ;IN1_ON;IN2_ON;IN3_ON;IN4_ON;

//-------------------------//

//==============================================================


//----------------===============METHODS============-------------------


void wait(unsigned long i){
 

 // if (i > 16380) {
 // times=millis()+i/1000;
 // while(millis()<times){
    
 //   }
  //}
 // else{
   times=micros()+i;
      while(micros()<times){
   // Serial.println(micros());
    }
}

//--------------Comparator--------------------
void init_comparator()
{
    // Disable the digital input buffers.
    DIDR1 = (1<<AIN1D) | (1<<AIN0D);
    
    // Setup the comparator...
    // Enabled, no bandgap, interrupt enabled,
    // no input capture, interrupt on falling edge.
   // ACSR = (1<<ACD) | (0<<ACBG) | (1<<ACIE) | (0<<ACIC) | (1<<ACIS1) | (0<<ACIS0);
    //ACSR  |= (1<<ACIE);
     ACSR = ( (1<<ACIE) | (1<<ACIS1) | (0<<ACIS0));
    
}

ISR(ANALOG_COMP_vect)
{
    // Did it just go high?
    if (ACSR & (1<<ACO))
    {
        // Indicate a problem.
        alarm = 0;

        // Change to falling edge.
        ACSR &= ~(1<<ACIS0);
       
    }
    else
    {
        // Indicate a all is well.
        //alarm = ;
   alarm=1 ;
        // Set up the rising edge.
        ACSR |= (1<<ACIS0);
    }
}

void detect(int i){
 // Serial.print(i);
  if(alarm){
   // Serial.print(" == HIGH");
    //Serial.print("1");
   muxValues[i]=1;
   // alarm=0;
    }
  else muxValues[i]=0; // Serial.print("0");//Serial.print(" == LOWw");
   
  
  }


void setPin(int outputPin)
// function to select pin on 74HC4067
{
  PORTD = controlPins[outputPin];
}

void updateIRArr(){
    for(int i=8;i<16;i++){
  setPin(i);
   wait(200);
  detect(i);
 }


}


byte detectIR(byte i){
  byte count=50;
  byte val;
  PORTD = controlPins[i];
  /*while(count>0){
    wait(200);
    if(alarm) val=1;
    else {
      val=0;
      break;
    }
   count--;
   }*/
   wait(200);
    if(alarm) return 1;
    else return 0;
 }

int getError(){

  updateIRArr();
  errCount=0;
  for(int i=9;i<12;i++){

      if(muxValues[i]==1){
      switch(i){
       // case 8 : errArr[0]=150;
        //break;
        case 9 : errArr[1]=200;
        break;
        case 10 : errArr[2]=250;
        break;
        case 11 : errArr[3]=300;
        break;
        //case 12: errArr[4]=350;
        //break;
      }
      errCount++;
      }
      
  
}
 error=0;
 for(int i=0;i<5;i++){
  error+=errArr[i];
  errArr[i]=0;
 }
 
 return error=(error/errCount)-250;
}

  



//-----------------------------------------------

//----------MotorControl-------------------------
void initMotors(){
  IN1;IN2;IN3;IN4;EN1;EN2;

TCCR1A=0x00;
TCCR1B=0x00;

TCCR2A=0x00;
TCCR2B=0x00;

TCCR2A |= (1<<COM2A1|1<<WGM21|1<<WGM20);
TCCR2B |= (1<<CS20);

TCCR1A |=(1<<COM1B1|1<<WGM10);
TCCR1B |= (1<<CS10|1<<WGM12);

//OCR2A =rpwm;
//OCR1B =lpwm;

 }

//-----------------------------------------------

//-------------------PID-------------------------

void pidFollow(int err){
 //error=getError();
//Serial.println(error);

  FORWD;
  

  mSpeed= kp*err + kd*(err-lastErr);
  lastErr=err;
  
  lpwm=rbSpeed +mSpeed; 
  rpwm=lbSpeed-mSpeed;


 
  if (rpwm > rMax) rpwm = rMax; // prevent the motor from going beyond max speed
  if (lpwm > lMax ) lpwm = lMax; // prevent the motor from going beyond max speed
  if (rpwm < 0) rpwm = 0; // keep the motor speed positive
  if (lpwm < 0) lpwm = 0; // keep the motor speed positive
  
  
 OCR2A = rpwm;
 OCR1B =lpwm;



wait(10000);
}

//-----------------------------------------------


//----------------Communication------------------

void startCom(){
    
  pinMode(A1,OUTPUT);//A1 transmit signals
  pinMode(A5,INPUT);//A5 reads receiver

  PCICR |= (1 << PCIE1);                                       //Set PCIE0 to enable PCMSK0 scan.
  PCMSK1 |= (1 << PCINT13);   

} 
 void stopCom(){
  
  PCICR |= (0 << PCIE1);                                       //Set PCIE0 to enable PCMSK0 scan.
  PCMSK1 |= (0 << PCINT13);
  pinMode(A5,OUTPUT);

  }

   void transmit(int pulse){
     PORTC|=(1<<1);
     wait(pulse);
     PORTC &=~(1<<1);
     wait(4000);
     }

  void waitForCol(){
    r1=0;
    startCom();
    while(r1<=1900 && r1>=2100){
     transmit(2000);
      }
     stopCom();
    }

  void waitforBox(){
     r1=0;
    startCom();
    while(r1<=1900 && r1>=2100){
     transmit(1500);
      }
     stopCom();
    }
    
    
    
     

  void waitForCallib(){
    DDRC &=~(1<<1);
    while(1){if(PINC & (1<<1))break;}
   Serial.println("recev start");
     DDRC|=(1<<1);
    }


void setPID(int value,int variable){
  float val=value;
  float var=variable;
 // if(variable<550 && variable >450)
 kp=(val-1000)/100;
  //if(variable<950 && variable >1050)
  kd=(var-1000)/10;
  
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

//-----------------------------------------------

//---------------------Driving--------------------------


void drive(char c){
 byte count=0;
 OCR2A =0;
 OCR1B =0;
 ALL_OFF;
 
 wait(5000);
  switch(c){
    
      case 'L': LEFT;
                OCR2A =turnSpeed;
                OCR1B =turnSpeed;
                //while(!detectIR(11));
                while(count<=maxCount)(detectIR(11))?count++:count=0;
                
                
      break;

     case 'A': LEFT;                         //4 way junct left turn
                OCR2A =turnSpeed;
                OCR1B =turnSpeed;
                while(count<=maxCount)(detectIR(8))?count++:count=0;
                count=0;
               // while(!detectIR(11));
                while(count<=maxCount)(detectIR(11))?count++:count=0;    

     break;


     case 'D': RIGHT;                         //4 way junct right turn
                OCR2A =turnSpeed;
                OCR1B = turnSpeed;
               // while(!detectIR(12));
                while(count<=maxCount)(detectIR(12))?count++:count=0;
                count=0;
               // while(!detectIR(9));
                while(count<=maxCount)(detectIR(9))?count++:count=0;                    

     break;
     

       case 'R': RIGHT;
                OCR2A = turnSpeed;
                OCR1B = turnSpeed;
              //  while(!detectIR(9));
                while(count<=maxCount)(detectIR(9))?count++:count=0;
                
      break;

       case 'F': FORWD;
                OCR2A =rbSpeed;
                OCR1B =lbSpeed;
      break;

       case 'B': REVS;            //Backward
                OCR2A =rbSpeed;
                OCR1B =lbSpeed;
      break;
    
  }

  if(c!='F' && c!='B'){
  BRAKE;
  OCR2A =0;
  OCR1B =0;
 //  BRAKE;
 // wait(5000);
 // ALL_OFF;
  }
  lastErr=0;
}


//------------------------------------------------------

//----------------==================================-------------------


void setup() {
 Serial.begin(9600);
//------comparator-------
init_comparator();
DDRD = B00111100;

//-----------------------

//------MotorControl-------
initMotors();

//-----------------------

//------Comm-------
//initPIDCom();

//-----------------------

sei();

waitForCallib();
}

void loop() {
  // put your main code here, to run repeatedly:

while(inMaze){  
  error=getError();

 if(!muxValues[10]){

         if(midCount==maxCount){
          midCount=0;
         //detect objects
          OCR2A = rbSpeed;
          OCR1B =lbSpeed;
          while(detectIR(14));
          drive('L');
         }
         else midCount++; 
  
  } 
  
 else if(muxValues[8] && muxValues[12]){                    //detects a crossing

        LCount=0;
        RCount=0;
        midCount=0;        
        OCR2A = rbSpeed;
        OCR1B =lbSpeed;
       while(!detectIR(13) || !detectIR(15) );
         
       if(detectIR(8) && detectIR(12)){ ALL_OFF ;  }       //end of maze  
       
       else if(detectIR(10))  {                       //4 way junct
           if(LFollow)drive('A');
           else drive('D');  
       }
       else {
           if(LFollow)drive('L');
           else drive('R'); 
       }                       
       
  }

  else if(muxValues[8]){                            
             RCount=0;
             midCount=0; 
         if(LCount==maxCount){
               LCount=0;
               
                
               OCR2A =rbSpeed;
               OCR1B =lbSpeed;
         
               while(!detectIR(13));
               Serial.println("detected 13");
               if(detectIR[10]){                      //3 way junct
                   if(LFollow)drive('A');
                   //else while(detectIR[13]);  
               }
                else drive('L');
                  
          LCount=0;
          
          }
          else LCount++;  
    
    }

  else if(muxValues[12]){
                         // RCount=0;
                           LCount=0;
                           midCount=0; 
                           //drive('F');
                           Serial.println("Found right");
             
         if(RCount==maxCount){
                           OCR2A =rbSpeed;
                           OCR1B =lbSpeed;
               Serial.println("count ok waiting for middle");
         
               while(!detectIR(15));
                 Serial.println("detected back right");
               if(detectIR(10)){                   //3 way junct
                   if(!LFollow)drive('D');
                   //else while();  
               }
                else 
                    drive('R'); //Serial.println("turned to right");}
          RCount=0;
          
          }
          else RCount++;  
    
    }

    else {pidFollow(error);}


}
//setPID(r1,r2);
//Serial.println(kd);
}
