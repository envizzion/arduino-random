#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/twi.h>

//Motoradresse
int MOTOR_ADR=7;


#define START_PWM   5

volatile unsigned long i2c_timeout = 0;
volatile unsigned int rx_pwm = 250;
volatile unsigned char rotor_state = 0;
volatile unsigned char rotor_run = 0;
void setup() {
  // put your setup code here, to run once:
pinMode(9,OUTPUT);
pinMode(8,OUTPUT);
//I2C Init
 TWAR = MOTOR_ADR & 0xFE;
  TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);
  
  //Interrupts enabel
  sei();
//pinMode(A1,INPUT);

}


ISR (TWI_vect)
//############################################################################
{  
  switch (TWSR & 0xF8) //TW_STATUS 
  {  
    //Adresse empfangen
    case TW_MT_SLA_ACK:  
      TWCR |= (1<<TWINT);
      return;
      
    //Daten empfangen
    case TW_MT_DATA_ACK:
      TWDR=rx_pwm;
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
}

















void loop() {
  // put your main code here, to run repeatedly:
rx_pwm=analogRead(A1);




 


}
