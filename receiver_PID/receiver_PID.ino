/* Test code for the Radio control transmitter
 *  Install the NRF24 library to your IDE
 * Upload this code to the Arduino UNO
 * Connect a NRF24 module to it:
 
    Module // Arduino UNO
    
    GND    ->   GND
    Vcc    ->   3.3V
    CE     ->   D9
    CSN    ->   D10
    CLK    ->   D13
    MOSI   ->   D11
    MISO   ->   D12

This code should print the received values to the serial monitor
Please, like share and subscribe : https://www.youtube.com/c/ELECTRONOOBS
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//#include <Servo.h>
//Servo esc,left,right;
const uint64_t pipeIn = 0xE8E8F0F0E1LL; //Remember that this code is the same as in the transmitter

int currRoll,currPitch,roll,pitch,throt;

double esc_1, esc_2, esc_3, esc_4;
int throttle, battery_voltage;
int cal_int, start, gyro_address;
int receiver_input[5];
unsigned long timer_channel_1, timer_channel_2, timer_channel_3, timer_channel_4, esc_timer, esc_loop_timer;
unsigned long timer_1, timer_2, timer_3, timer_4, current_time;
byte last_channel_1, last_channel_2, last_channel_3, last_channel_4;
unsigned long loop_timer;


RF24 radio(9, 10); 

//We could use up to 32 channels
struct MyData {
uint16_t throttle; //We define each byte of data input, in this case just 6 channels
uint16_t yaw;
uint16_t pitch;
uint16_t roll;
uint16_t AUX1;
uint16_t AUX2;
};

MyData data;

void resetData()
{
//We define the inicial value of each data input
//3 potenciometers will be in the middle position so 127 is the middle from 254
data.throttle = 1000;
data.yaw = 127;
data.pitch = 1500;
data.roll = 1500;
data.AUX1 = 0;
data.AUX2 = 0;

}

/**************************************************/

void setup()
{
Serial.begin(9600); //Set the speed to 9600 bauds if you want.
//You should always have the same speed selected in the serial monitor
resetData();
radio.begin();
radio.setAutoAck(false);
radio.setDataRate(RF24_250KBPS);

radio.openReadingPipe(1,pipeIn);
//we start the radio comunication
radio.startListening();
 DDRD |= B11110000;


}

/**************************************************/

unsigned long lastRecvTime = 0;

void recvData()
{
while ( radio.available() ) {
radio.read(&data, sizeof(MyData));
lastRecvTime = millis(); //here we receive the data
}
}

/**************************************************/

void loop()
{
recvData();
unsigned long now = millis();
unsigned long curr=micros();
//Here we check if we've lost signal, if we did we reset the values 
if ( now - lastRecvTime > 2000 ) {
// Signal lost?
resetData();
}

Serial.print("Throttle: "); Serial.print(esc_1);  Serial.print("    ");
Serial.print("Yaw: ");      Serial.print(esc_2);       Serial.print("    ");
Serial.print("Pitch: ");    Serial.print(esc_3);     Serial.print("    ");
Serial.print("Roll: ");     Serial.print(esc_4);      Serial.print("    ");
Serial.print("Aux1: ");     Serial.print(data.AUX1);      Serial.print("    ");
Serial.print("Aux2: ");     Serial.print(data.AUX2);      Serial.print("\n");
esc_4=data.throttle;
esc_2=data.pitch;
esc_3=data.roll;
esc_1=500;

 //esc_2=esc_2/10;
//sc_3=esc_3/100;
 //esc_4=esc_4/100;
 

 while(micros() - loop_timer < 4000);                                      //We wait until 4000us are passed.
  loop_timer = micros();                                                    //Set the timer for the next loop.

  PORTD |= B11111000;                                                       //Set digital outputs 4,5,6 and 7 high.
  timer_channel_1 = esc_1 + loop_timer;                                     //Calculate the time of the faling edge of the esc-1 pulse.
  timer_channel_2 = esc_2 + loop_timer;                                     //Calculate the time of the faling edge of the esc-2 pulse.
  timer_channel_3 = esc_3 + loop_timer;                                     //Calculate the time of the faling edge of the esc-3 pulse.
  timer_channel_4 = esc_4 + loop_timer;                                     //Calculate the time of the faling edge of the esc-4 pulse.
  
  while(PORTD >= 16){                                                       //Stay in this loop until output 4,5,6 and 7 are low.
    esc_loop_timer = micros();                                              //Read the current time.
    if(timer_channel_1 <= esc_loop_timer)PORTD &= B11101111;                //Set digital output 4 to low if the time is expired.
    if(timer_channel_2 <= esc_loop_timer)PORTD &= B11011111;                //Set digital output 5 to low if the time is expired.
    if(timer_channel_3 <= esc_loop_timer)PORTD &= B10111111;                //Set digital output 6 to low if the time is expired.
  if(timer_channel_4 <= esc_loop_timer)PORTD &= B01111111;                //Set digital output 7 to low if the time is expired.
  }

 
}




//PORTD |= (1<<7);
//delayMicroseconds(throt);
//PORTD &= ~(1<<7);  
//esc.writeMicroseconds(throt);


//servo value change




/**************************************************/
