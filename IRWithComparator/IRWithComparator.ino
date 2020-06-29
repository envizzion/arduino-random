#include <avr/io.h>
#include <avr/interrupt.h>

unsigned long times=0;
volatile int alarm = 0;
int errArr[]={0,0,0,0,0};
int error=0;
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

void init_comp()
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

void setup(){
  Serial.begin(9600);
  init_comp();

  DDRD = B00111100;
   
  while(0){
  
  
    
    }
  
  }

  void loop(){



    for(int i=8;i<16;i++){
  setPin(i);
   wait(200);
  detect(i);
  //Serial.print(" ");
  
}
    for(int i=8;i<16;i++){

      if(muxValues[i]==1 && i<13){
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
      
  Serial.print(muxValues[i]);
   //wait(20);
  //detect(i);
  Serial.print(" ");
  
}
 error=0;
 for(int i=0;i<5;i++){
  error+=errArr[i];
  errArr[i]=0;
 }
 
 error=error-2500;
 Serial.print(" error="); 
 Serial.println(error);
    
    
    }
      
