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
#include <Servo.h>
Servo variable,value;
const uint64_t pipeIn = 0xE8E8F0F0E1LL; //Remember that this code is the same as in the transmitter

int currRoll,currPitch,roll,pitch,throt;

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
pinMode(A0,OUTPUT);
pinMode(A5,OUTPUT);
//DDRC =B00100001;
Serial.begin(9600); //Set the speed to 9600 bauds if you want.
//You should always have the same speed selected in the serial monitor
resetData();
radio.begin();
radio.setAutoAck(false);
radio.setDataRate(RF24_250KBPS);

radio.openReadingPipe(1,pipeIn);
//we start the radio comunication
radio.startListening();
variable.attach(A0);
value.attach(A5);
//right.attach(5);
//left.write(90);
//right.write(90);


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
if ( now - lastRecvTime > 1000 ) {
// Signal lost?
resetData();
}

Serial.print("kp: "); Serial.print(data.throttle);  Serial.print("    ");
Serial.print("kd: ");      Serial.print(data.yaw);       Serial.println("    ");
//Serial.print("Pitch: ");    Serial.print(data.pitch);     Serial.print("    ");
//Serial.print("Roll: ");     Serial.print(data.roll);      Serial.print("    ");
//Serial.print("Aux1: ");     Serial.print(data.AUX1);      Serial.print("    ");
//Serial.print("Aux2: ");     Serial.print(data.AUX2);      Serial.print("\n");
throt=data.throttle;
currPitch=data.pitch;
currRoll=data.roll;

//PORTC |= (1<<5);
//delayMicroseconds(data.yaw+1000);
//PORTC &= ~(1<<5);  
value.writeMicroseconds(data.yaw+1000);
variable.writeMicroseconds(data.throttle+1000);
delay(1000);

//servo value change


}

/**************************************************/
