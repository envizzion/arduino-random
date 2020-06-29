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
#define UH_DDR  DDRB |= (1<<3);
#define UH_ON TCCR2A |= (1<<COM2A1);
#define UH_OFF  TCCR2A &= ~(1<<COM2A1);

//PHASE1 (U)
#define UL_DDR  DDRB |= (1<<1);
#define UL_ON PORTB |= (1<<1);
#define UL_OFF  PORTB &= ~(1<<1);


//PHASE2 (V)
#define VH_DDR  DDRD |= (1<<5);
#define VH_ON TCCR0A |= (1<<COM0B1);
#define VH_OFF  TCCR0A &= ~(1<<COM0B1);

//PHASE2 (V)
#define VL_DDR  DDRD |= (1<<3);//define VL_DDR  DDRB |= (1<<2);
#define VL_ON PORTD |= (1<<3);//#define VL_ON PORTB |= (1<<2);
#define VL_OFF  PORTD &= ~(1<<3);//#define VL_OFF  PORTB &= ~(1<<2);


//PHASE3 (W)
#define WH_DDR  DDRD |= (1<<6);//#define WH_DDR  DDRD |= (1<<3);
#define WH_ON TCCR2A |= (1<<COM0A1);//#define WH_ON TCCR2A |= (1<<COM2B1);
#define WH_OFF  TCCR2A &= ~(1<<COM0A1);//#define WH_OFF  TCCR2A &= ~(1<<COM2B1);

//PHASE3 (W)
#define WL_DDR  DDRD |= (1<<4);//#define WL_DDR  DDRC |= (1<<3);
#define WL_ON PORTD |= (1<<4);//#define WL_ON PORTC |= (1<<3);
#define WL_OFF  PORTD &= ~(1<<4);//#define WL_OFF  PORTC &= ~(1<<3);


#define PHASE_ALL_OFF UH_OFF;UL_OFF;VH_OFF;VL_OFF;WH_OFF;WL_OFF;

#define SENSE_U   ADMUX = 0;
#define SENSE_V   ADMUX = 1;
#define SENSE_W   ADMUX = 2;

#define SENSE_H   (ACSR&(1<<ACO))

#define START_PWM   5

volatile unsigned long i2c_timeout = 0;
volatile unsigned char rx_pwm = 100;
volatile unsigned char rotor_state = 0;
volatile unsigned char rotor_run = 0;

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
        TCNT1 = 1;
      }
      break;

    case (1):
      if(SENSE_H || startup)
      {
        VL_OFF;
        WL_ON;
        SENSE_V;
        rotor_state = 2;
        TCNT1 = 1;
      }
      break;

    case (2):
      if(!SENSE_H || startup)
      {
        UH_OFF;
        VH_ON;
        SENSE_U;
        rotor_state = 3;
        TCNT1 = 1;
      }
      break;
  
    case (3):
      if(SENSE_H || startup)
      {
        WL_OFF;
        UL_ON;
        SENSE_W;
        rotor_state = 4;
        TCNT1 = 1;
      }
      break;

    case (4):
      if(!SENSE_H || startup)
      {
        VH_OFF;
        WH_ON;
        SENSE_V;
        rotor_state = 5;
        TCNT1 = 1;
      }
      break;

    case (5):
      if(SENSE_H || startup)
      {
        UL_OFF;
        VL_ON;
        SENSE_U;
        rotor_state = 0;
        TCNT1 = 1;
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
  next_commutate_state (1);
  rotor_run = 0;
  OCR2A = START_PWM;
  OCR0A = START_PWM;//  OCR2B = START_PWM;
  OCR0B = START_PWM;
}

//############################################################################
ISR (TWI_vect)
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
      rx_pwm=analogRead(A5);
      Serial.println(rx_pwm);
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
}

//############################################################################
//Hauptprogramm
int main (void) 
//############################################################################
{
  //Watchdog on
  WDTCSR = (1<<WDCE) | (1<<WDE);
  Serial.begin(250000);
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
  TWAR = MOTOR_ADR & 0xFE;
    TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);
  
  //Interrupts enabel
  sei();
  

  while(1)
  { 
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
      i2c_timeout++;
      
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


