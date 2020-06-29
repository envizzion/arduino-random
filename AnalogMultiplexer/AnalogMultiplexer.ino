byte controlPins[] = {B00000000, 
                  B00100000,
                  B00010000,
                  B00110000,
                  B00001000,
                  B00101000,
                  B00011000,
                  B00111000,
                  B00000100,
                  B00100100,
                  B00010100,
                  B00110100,
                  B00001100,
                  B00101100,
                  B00011100,
                  B00111100 }; 
 
// holds incoming values from 74HC4067                  
byte muxValues[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
 
void setup()
{
  Serial.begin(9600);
  DDRC = B00011110; // set PORTD (digital 7~0) to outputs
setPin(0);
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
    Serial.print("input I"); 
    Serial.print(i); 
    Serial.print(" = "); 
    Serial.println(muxValues[i]);
  }
  Serial.println("========================");  
}
 
void loop()
{
  /*
  for (int i = 0; i < 16; i++)
  {
    setPin(i); // choose an input pin on the 74HC4067
    muxValues[i]=analogRead(0); // read the vlaue on that pin and store in array
  }
 
  // display captured data
  displayData();
  delay(2000); 
  */
  
  Serial.println(A0);
}
