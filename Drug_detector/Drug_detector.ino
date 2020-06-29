const int AOUTpin=0;//the AOUT pin of the alcohol sensor goes into analog pin A0 of the arduino
const int DOUTpin=8;//the DOUT pin of the alcohol sensor goes into digital pin D8 of the arduino
const int ledPin = 13 ; //the anode of the LED connects to digital pin D13 of the arduino

int limit;
int value;
int hallsensor = 2;                // Hall sensor at pin 2

volatile long  counter;

float rpm;

 float passedtime;


void isr()

 {

   //Each rotation, this interrupt function is run twice, so take that into consideration for

   //calculating RPM

   //Update count

      counter++;

 }


void setup() {
Serial.begin(9600);//sets the baud rate

//--Alcohol sensor setup--//
pinMode(DOUTpin, INPUT);//sets the pin as an input to the arduino
pinMode(ledPin, OUTPUT);//sets the pin as an output of the arduino
//----------------------//
pinMode(A1,INPUT);
pinMode(A2,INPUT);
pinMode(A0,INPUT);
//----Hall sensor setup-----//
attachInterrupt(0, isr, RISING); //Interrupts are called on Rise of Input

   pinMode(hallsensor, INPUT); //Sets hallsensor as input

   counter = 0;

   rpm = 0;

   passedtime = 0; //Initialise the values

//----------------------//



}

void loop()
{


//-----alcohol sensor---//  
value= analogRead(AOUTpin);//reads the analaog value from the alcohol sensor's AOUT pin
limit= digitalRead(DOUTpin);//reads the digital value from the alcohol sensor's DOUT pin
Serial.print("  Alcohol value: ");
Serial.print(value);//prints the alcohol value




//--------Temperature sensor----------//
Serial.print("  Temp: ");

int temp=analogRead(A2);
temp=-1*(temp*500)/1024;


Serial.println(temp);
//---------------//







Serial.print("Limit: ");
Serial.print(limit);//prints the limit reached as either LOW or HIGH (above or underneath)
//delay(100);
if (limit == HIGH){
digitalWrite(ledPin, HIGH);//if limit has been reached, LED turns on as status indicator

}
else{
digitalWrite(ledPin, LOW);//if threshold not reached, LED remains off

}

//-------------------//






//----Hall Sensor-----//


if ((millis()-passedtime)>10){//Update RPM every second

 detachInterrupt(0); //Interrupts are disabled

 rpm = 60*1000/(millis() - passedtime)*counter;

 passedtime = millis();

 counter = 0;

 Serial.print("  RPM= ");

//Serial.print(analogRead(A1)); //Print out result to monitor
Serial.print(rpm); 
attachInterrupt(0, isr, RISING);   //Restart the interrupt processing

//---------------//


}







}

