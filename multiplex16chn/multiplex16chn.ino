// control pins output table in array form
// see truth table on page 2 of TI 74HC4067 data sheet
// connect 74HC4067 S0~S3 to Arduino D7~D4 respectively
// connect 74HC4067 pin 1 to Arduino A0
int maxx=0,minn=0,avg,avgPWM;
byte controlPins[] = {B00000000, 
                  B00000010,
                  B00000100,
                  B00000110,
                  B00001000,
                  B00001010,
                  B00001100,
                  B00001110,
                  B00010000,
                  B00010010,
                  B00010100,
                  B00010110,
                  B00011000,
                  B00011010,
                  B00011100,
                  B00011110 }; 
 
// holds incoming values from 74HC4067                  
int muxValues[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
 



void setup()
{
  Serial.begin(9600);
DDRC = B00011110; // set PORTD (digital 7~0) to outputs
 // setPin(0);
DDRD |=(1<<3);
TCCR2A=0x00;
TCCR2B=0x00;

TCCR2A |= (1<<COM2B1|1<<WGM21|1<<WGM20);
TCCR2B |= (1<<CS20);
//analogWrite(3,128);
}

 
void setPin(int outputPin)
// function to select pin on 74HC4067
{
  PORTC = controlPins[outputPin];
}
 
void displayData()
// dumps captured data from array to serial monitor
{
  Serial.println();
  Serial.println("Values from multiplexer:");
  Serial.println("========================");
  for (int i = 0; i < 16; i++)
  {
   // setPin(i);
    Serial.print("input I"); 
    Serial.print(i); 
    Serial.print(" = "); 
    Serial.println(muxValues[i]);
    if(muxValues[i]<minn)minn=muxValues[i];
    if(muxValues[i]>maxx)maxx=muxValues[i];
    
    
  }
  Serial.print("avg= ");
  avg=(minn+maxx)/2;
    Serial.print(avg);
  Serial.print("  avgPWM=");
  avgPWM=avg/4;
  Serial.print(avgPWM);
  Serial.print("  PWMRead= ");
  Serial.print(analogRead(A5));
   Serial.println("========================");  
}
 
void loop()
{
  
 // Serial.println(analogRead(A0));
for(int i=8;i<16;i++){
  setPin(i);
muxValues[i]=analogRead(A6);

}
    displayData();
    OCR2B=255;
  delay(1000);
 // 
 /*
  setPin(2);
 Serial.print(analogRead(A0));
 delay(100);
//  setPin(6);
 Serial.print("  ");
 Serial.print(analogRead(A1));

 delay(100);
//  setPin(5);
 Serial.print("g  ");
 Serial.print(analogRead(A0));
 Serial.println();
  delay(100);
 
 // }
 */
 
}  

