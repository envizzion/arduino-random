//Aircraft Stabilizer
//Servo 
#include <Servo.h>
#include "MPU6050_6Axis_MotionApps20.h"
#include "I2Cdev.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

Servo myservoY; // Roll
Servo myservoX; // Pitch

MPU6050 mpu;
//MPU6050 mpu(0x69); // <-- use for AD0 high

#define OUTPUT_READABLE_YAWPITCHROLL

#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y ]         quaternion container
VectorInt16 aa;         // [x, y]            accel sensor measurements
VectorInt16 aaReal;     // [x, y]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y]            gravity vector
float euler[3];         // [psi, theta]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };



// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = false;
}



// ================================================================
// ===                      RECEIVER INITIALIZATION                    ===
// ================================================================



#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

Servo esc;//,left,right;

const uint64_t pipeIn = 0xE8E8F0F0E1LL; //Remember that this code is the same as in the transmitter

int currRoll,currPitch,roll,pitch,throt;

RF24 radio(9, 10); 

//We could use up to 32 channels
struct MyData {
int16_t throttle; //We define each byte of data input, in this case just 6 channels
int16_t yaw;
int16_t pitch;
int16_t roll;
byte AUX1;
byte AUX2;
};

MyData data;

void resetData()
{
//We define the inicial value of each data input
//3 potenciometers will be in the middle position so 127 is the middle from 254
data.throttle = 1000;
//data.yaw = 0;
data.pitch = 0;
data.roll = 0;
data.AUX1 = 1;
data.AUX2 = 0;

}

unsigned long prevT,currT,diffT;
int16_t currPAng,currRAng;
int16_t serYVal,serXVal;
boolean recFlag,pitchOK;
boolean rollSetFlag=false;
/**************************************************/













// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

//Attach servo

  myservoY.attach(5); // Attach Y servo to pin 9
  myservoX.attach(6);// Attach X servo to pin 10
  esc.attach(3);
  myservoY.write(90);
  myservoX.write(90);

    // initialize serial communication
    // (115200 chosen because it is required for Teapot Demo output, but it's
    // really up to you depending on your project)
    Serial.begin(115200);
    while (!Serial); // wait for Leonardo enumeration, others continue immediately

    // NOTE: 8MHz or slower host processors, like the Teensy @ 3.3v or Ardunio
    // Pro Mini running at 3.3v, cannot handle this baud rate reliably due to
    // the baud timing being too misaligned with processor ticks. You must use
    // 38400 or slower in these cases, or use some kind of external separate
    // crystal solution for the UART timer.

    // initialize device
    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
/*
    // wait for ready
    Serial.println(F("\nSend any character to begin DMP programming and demo: "));
    while (Serial.available() && Serial.read()); // empty buffer
    while (!Serial.available());                 // wait for data
    while (Serial.available() && Serial.read()); // empty buffer again
*/
    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

    // configure LED for output
    pinMode(LED_PIN, OUTPUT);

//-----RECEIVER SETUP---//
resetData();
radio.begin();
radio.setAutoAck(false);
radio.setDataRate(RF24_250KBPS);

radio.openReadingPipe(1,pipeIn);
//we start the radio comunication
radio.startListening();






    
}



/****************Receiver reset**********************************/

unsigned long lastRecvTime = 0;

void recvData()
{
while ( radio.available() ) {
radio.read(&data, sizeof(MyData));
lastRecvTime = millis(); //here we receive the data
}
}

/**************************************************/




// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
        // other program behavior stuff here
        // .
        // .
        // .
        // if you are really paranoid you can frequently test in between other
        // stuff to see if mpuInterrupt is true, and if so, "break;" from the
        // while() loop to immediately process the MPU data
        // .
        // .

      
   
     recvData();
     unsigned long now = millis();
     unsigned long curr=micros();
     //Here we check if we've lost signal, if we did we reset the values 
     if ( now - lastRecvTime > 1000 ) {
     // Signal lost?
     resetData();
     }

     Serial.print("Throttle: "); Serial.print(data.throttle);  Serial.print("    ");
    
     Serial.print("Pitch: ");    Serial.print(data.pitch);     Serial.print("    ");
     Serial.print("Roll: ");     Serial.print(data.roll);      Serial.print("    ");
     Serial.print("Aux1: ");     Serial.print(data.AUX1);      Serial.print("    ");
     Serial.print("Aux2: ");     Serial.print(data.AUX2);      Serial.print("\n");
     throt=data.throttle;
     currPitch=data.pitch;
     currRoll=data.roll;

     //PORTD |= (1<<7);
     //delayMicroseconds(throt);
     //PORTD &= ~(1<<7);  
     esc.writeMicroseconds(throt);


//servo value change

  //----idle control reset ----//
      currT=millis();  
     if(currRoll==0 && currPitch==0 ){
      diffT=currT-prevT;
      if(diffT>1500){
        break;
      }
      
      
     }
     else{ 
      prevT=currT;
     
    }
    
              serYVal=currPitch + currRoll;
              serXVal=-currPitch + currRoll; 
             
              myservoY.write(serYVal+90);
              myservoX.write(serXVal+90);

    
     

    //-----//
} 
      

     
     
         
        
        
       
    

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

            currRAng=int(ypr[1] * 180/M_PI);
            currPAng=int(ypr[2] * 180/M_PI);

             /* //if(recFlag==false){
           
               if((currRAng>2 || currRAng<-2) && pitchOK){
                 serVal=currRAng;
                } 
              else if(currPAng>5|| currPAng <-5 ){
                 serVal=currPAng;
                 pitchOK=false;
              }   
                
               else
                pitchOK=true;
            //}
            //else{
              //serVal=currPitch+currRoll; 
              
             // }
            */
            
        
            if(currRAng!=0   && !rollSetFlag ){
             serYVal=currRAng;
             serXVal=serYVal;
             
            }
            else{rollSetFlag=true;}

            if(rollSetFlag && currPAng!=0 ){
              
              serYVal=currPAng;
              serXVal=-serYVal;
              }
            else{rollSetFlag=false;}

            

           
            
            Serial.print("ypr\t");
            Serial.print(ypr[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr[1] * 180/M_PI);
            
           // myservoY.write(int(ypr[2] * 180/M_PI)+90);   // Rotation around Y
            myservoY.write(serYVal+90);
            Serial.print("\t");
            Serial.println(ypr[2] * 180/M_PI);
            //myservoX.write(int(ypr[2] * -180/M_PI)+90);   // Rotation around X
            myservoX.write(serXVal+90);
        #endif


        // blink LED to indicate activity
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);
    }
    
}

