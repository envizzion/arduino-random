#include <avr/io.h>
#include <avr/interrupt.h>

//========VARIABLES===============================
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
int error=0,lastErr=0;
float kp=0.2 ,kd=5,ki=0;

//--------------

//-------motorControl------//
int rbSpeed=150,rMax=255;
int lbSpeed=150,lMax=255;
int mSpeed=0;
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

#define FORWD ; IN1_ON; IN2_OFF; IN3_ON; IN4_OFF;

#define REVS ;IN1_OFF;IN2_ON;IN3_OFF;IN4_ON;

#define LEFT;IN1_OFF;IN2_ON; IN3_ON; IN4_OFF;

#define RIGHT ; IN1_ON; IN2_OFF;IN3_OFF;IN4_ON;

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
    sei();
}

ISR(ANALOG_COMP_vect)
{
    // Did it just go high?
    if (ACSR & (1<<ACO))
    {
        // Indicate a problem.
        alarm = 1;

        // Change to falling edge.
        ACSR &= ~(1<<ACIS0);
       
    }
    else
    {
        // Indicate a all is well.
        //alarm = ;
   alarm=0;
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

int getError(){

  updateIRArr();
  for(int i=8;i<13;i++){

      if(muxValues[i]==1){
      switch(i){
        case 8 : errArr[0]=1500;
        break;
        case 9 : errArr[1]=2000;
        break;
        case 10 : errArr[2]=2500;
        break;
        case 11 : errArr[3]=3000;
        break;
        case 12: errArr[4]=3500;
        break;
      }
     
      }
      
  
}
 error=0;
 for(int i=0;i<5;i++){
  error+=errArr[i];
  errArr[i]=0;
 }
 
 return error=error-2500;
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

void pidFollow(){
 error=getError();
  
  mSpeed= kp*error + kd*(error-lastErr);
  lastErr=error;
  
  rpwm=rbSpeed +mSpeed; 
  lpwm=lbSpeed-mSpeed;

 OCR2A =rpwm;
 OCR1B =lpwm;
 
  if (rpwm > rMax) rpwm = rMax; // prevent the motor from going beyond max speed
  if (lpwm > lMax ) lpwm = lMax; // prevent the motor from going beyond max speed
  if (rpwm < 0) rpwm = 0; // keep the motor speed positive
  if (lpwm < 0) lpwm = 0; // keep the motor speed positive

  //FORWD;
  IN1_ON;
IN2_OFF;
IN3_ON;
IN4_OFF;
wait(10000);
}

//-----------------------------------------------



//----------------==================================-------------------


void setup() {
  
//------comparator-------
init_comparator();
DDRD = B00111100;

//-----------------------

//------MotorControl-------
initMotors();

//-----------------------


}

void loop() {
  // put your main code here, to run repeatedly:
pidFollow();
}
