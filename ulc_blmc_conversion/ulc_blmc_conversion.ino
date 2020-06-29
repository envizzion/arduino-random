/*----------------------------------------------------------------------------
 Copyright:      Ulrich Radig (mail@ulrichradig.de)
 Author:         Ulrich Radig
 Remarks:        
 known Problems: none
 Version:        23.06.2011
 Description:    Brushless Motor Controller for ATmega48/88/168
 
 Dieses Programm ist freie Software. Sie können es unter den Bedingungen der 
 GNU General Public License, wie von der Free Software Foundation veröffentlicht, 
 weitergeben und/oder modifizieren, entweder gemäß Version 2 der Lizenz oder 
 (nach Ihrer Option) jeder späteren Version. 

 Die Veröffentlichung dieses Programms erfolgt in der Hoffnung, 
 daß es Ihnen von Nutzen sein wird, aber OHNE IRGENDEINE GARANTIE, 
 sogar ohne die implizite Garantie der MARKTREIFE oder der VERWENDBARKEIT 
 FÜR EINEN BESTIMMTEN ZWECK. Details finden Sie in der GNU General Public License. 

 Sie sollten eine Kopie der GNU General Public License zusammen mit diesem 
 Programm erhalten haben. 
 Falls nicht, schreiben Sie an die Free Software Foundation, 
 Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA. 
------------------------------------------------------------------------------*/

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/twi.h>

//Motoradresse
#define MOTOR_ADR 0x70

//PHASE1 (U)
#define UH_DDR  DDRD |= (1<<5);
#define UH_ON   TCCR0A |= (1<<COM0B1);
#define UH_OFF  TCCR0A &= ~(1<<COM0B1);

//PHASE1 (U)
#define UL_DDR  DDRB |= (1<<4);
#define UL_ON PORTB |= (1<<4);
#define UL_OFF  PORTB &= ~(1<<4);


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


#define PHASE_ALL_OFF UH_OFF;UL_OFF;VH_OFF;VL_OFF;WH_OFF;WL_OFF;

#define SENSE_U   ADMUX = 0;
#define SENSE_V   ADMUX = 1;
#define SENSE_W   ADMUX = 2;

#define SENSE_H   (ACSR&(1<<ACO))

#define START_PWM   5

volatile unsigned long i2c_timeout = 0;
volatile unsigned long rx_pwm =255;
volatile unsigned char rotor_state = 0;
volatile unsigned char rotor_run = 0;
volatile unsigned char stopTiming=0;

volatile unsigned long current_time,timer_1,prev_time,tmp_pwm=0;
volatile byte last_channel_1;
volatile int counter;
//############################################################################
void next_commutate_state (unsigned char startup)
//############################################################################
{
 
  switch (rotor_state)
  {
    case (0):
      if(!SENSE_H || startup)
      {
        WH_OFF;
        UH_ON;
        SENSE_W;
        rotor_state = 1;
       // TCNT1 = 1;
      }
      break;

    case (1):
      if(SENSE_H || startup)
      {
        VL_OFF;
        WL_ON;
        SENSE_V;
        rotor_state = 2;
      //  TCNT1 = 1;
      }
      break;

    case (2):
      if(!SENSE_H || startup)
      {
        UH_OFF;
        VH_ON;
        SENSE_U;
        rotor_state = 3;
       // TCNT1 = 1;
      }
      break;
  
    case (3):
      if(SENSE_H || startup)
      {
        WL_OFF;
        UL_ON;
        SENSE_W;
        rotor_state = 4;
       // TCNT1 = 1;
      }
      break;

    case (4):
      if(!SENSE_H || startup)
      {
        VH_OFF;
        WH_ON;
        SENSE_V;
        rotor_state = 5;
      //  TCNT1 = 1;
      }
      break;

    case (5):
      if(SENSE_H || startup)
      {
        UL_OFF;
        VL_ON;
        SENSE_U;
        rotor_state = 0;
      //  TCNT1 = 1;
      }
      break;
  }

}

//############################################################################
//back EMF zero crossing detection
ISR (ANALOG_COMP_vect) 
//############################################################################
{
  if(rotor_run == 200) next_commutate_state (0);
  
  rotor_run++;
  if(rotor_run > 200)
  {
    rotor_run = 200;
  }
}

//############################################################################
ISR (TIMER1_OVF_vect)
//############################################################################
{ 
  if(rotor_run<200){
  next_commutate_state (1);
  rotor_run = 0;
  OCR2A = START_PWM;
  OCR2B = START_PWM;
  OCR0B = START_PWM;
  }
}

//############################################################################
/*ISR (TWI_vect)
//############################################################################
{ 
  switch (TWSR & 0xF8) //TW_STATUS 
  {  
    //Adresse empfangen
    case TW_SR_SLA_ACK:  
      TWCR |= (1<<TWINT);
      return;
      
    //Daten empfangen
    case TW_SR_DATA_ACK:
      rx_pwm = TWDR;
      TWCR |= (1<<TWINT);
      i2c_timeout = 0;
      return;

    //Bus-Fehler zurücksetzen 
    case TW_NO_INFO:
      TWCR |=(1<<TWSTO) | (1<<TWINT); 

    //Bus-Fehler zurücksetzen   
    case TW_BUS_ERROR:
      TWCR |=(1<<TWSTO) | (1<<TWINT); 
  }
  //Reset TW
  TWCR =(1<<TWEA) | (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
}*/


ISR(PCINT2_vect){

 
  
  if(rotor_run>=200){

    current_time=TCNT1/2;
    //current_time=(current_time/2000)*250;
    
  //Channel 1=========================================
  if(PIND & B00000001){                                        //Is input 8 high?
    if(last_channel_1 == 0){                                   //Input 8 changed from 0 to 1
      last_channel_1 = 1;                                      //Remember current input state
      timer_1 = current_time;                                  //Set timer_1 to current_time
    }
  }
  else if(last_channel_1 == 1){                                //Input 8 is not high and changed from 1 to 0
    last_channel_1 = 0;                                        //Remember current input state
     tmp_pwm+= ((current_time - timer_1)-1000);                //Channel 1 is current_time - timer_1
   
    
      //tmp_pwm+=current_time-timer_1;              //Channel 1 is current_time - timer_1
    counter++;
    if(counter==5){
      tmp_pwm=tmp_pwm/20;
      if(tmp_pwm<rx_pwm- 5|| tmp_pwm>rx_pwm+5){rx_pwm=tmp_pwm;}
      tmp_pwm=0;
      counter=0;
      //TCNT1=1;
      }
   
   TCNT1=1;
   }
  }
}


//############################################################################
//Hauptprogramm
int main (void) 
//############################################################################
{
  //Serial.begin(250000);
  //Watchdog on
  WDTCSR = (1<<WDCE) | (1<<WDE);
  
  //Motordriver output
  UH_DDR;
  VH_DDR;
  WH_DDR;
  UL_DDR;
  VL_DDR;
  WL_DDR;
    
  //PWM for UH, VH and WH (>32KHz)
  TCCR0A |= (1<<COM0B1|1<<WGM01|1<<WGM00);
  TCCR0B |= (1<<CS00);
  
  TCCR2A |= (1<<COM2A1|1<<COM2B1|1<<WGM21|1<<WGM20);
  TCCR2B |= (1<<CS20);
  
  //TIMER1 for start commutation or Back EMF lost
  TCCR1B |= (1<<CS11);
  TIMSK1 |= (1<<TOIE1);

  PHASE_ALL_OFF;
  
  //Comperator init for back EMF
  ADCSRB  |= (1<<ACME);
  DIDR1 |= (1<<AIN0D);
  ACSR  |= (1<<ACIE);
    
  //I2C Init
 // TWAR = MOTOR_ADR & 0xFE;
   // TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);
  PCICR |= (1 << PCIE2);                                       //Set PCIE0 to enable PCMSK0 scan.
  ;PCMSK2 |= (1 << PCINT16); 
  //Interrupts enabel
  sei();
  

  while(1)
  { 
   //Serial.println(rx_pwm);
    asm("wdr");
    if(rx_pwm > START_PWM)
    {
      ACSR |= (1<<ACIE);
      TIMSK1 |= (1<<TOIE1);
      
      if(rotor_run == 200)
      {
        OCR2A = rx_pwm;
        OCR2B = rx_pwm;
        OCR0B = rx_pwm;
      }
      //i2c_timeout++;
      
      if(i2c_timeout>100000)
      {
        rx_pwm = 0;
      }
    }
    else
    {
      PHASE_ALL_OFF;
      ACSR&=~(1<<ACIE);
      TIMSK1 &= ~(1<<TOIE1);
    }
  }
}


