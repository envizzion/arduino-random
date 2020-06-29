int OutPut= 10;//naming pin10 of uno as output

unsigned int frequency = 0;

 

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins

LiquidCrystal lcd(8, 9, 7, 11, 12, 13);//RS,EN,D4,D5,D6,D7

 

void setup()

{
Serial.begin(9600);
                // lowest frequency for dominent color

                pinMode(2, OUTPUT);

                pinMode(3, OUTPUT);//PINS 2, 3,4,5 as OUTPUT

                pinMode(4, OUTPUT);

                pinMode(5, OUTPUT);

                pinMode(10, INPUT);//PIN 10 as input

 

                digitalWrite(2,HIGH);

                digitalWrite(3,LOW);//setting frequency selection to 20%

}

void loop()

{

                Serial.print("R=");//printing name

                digitalWrite(4,LOW);

                digitalWrite(5,LOW);//setting for RED color sensor

                frequency = pulseIn(OutPut, LOW);//reading frequency

                Serial.print(frequency);//printing RED color frequency

                Serial.println("  ");

                

                delay(500);

               

               Serial.print("B=");// printing name

                digitalWrite(4,LOW);

                digitalWrite(5,HIGH);// setting for BLUE color sensor

                frequency = pulseIn(OutPut, LOW);// reading frequency

                Serial.print(frequency);// printing BLUE color frequency

                Serial.println("  ");

                

                delay(500);

               

               Serial.print("G=");// printing name

                digitalWrite(4,HIGH);

                digitalWrite(5,HIGH);// setting for GREEN color sensor

                frequency = pulseIn(OutPut, LOW);// reading frequency

                Serial.print(frequency);// printing GREEN color frequency

                Serial.println("    ");

               

                delay(500);        

}              

