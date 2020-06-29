volatile int count=0,times=0;

void setup() {
pinMode(A5,OUTPUT);
  // put your setup code here, to run once:
 PCICR |= (1 << PCIE0);                             // set PCIE0 to enable PCMSK0 scan
  PCMSK0 |= (1 << PCINT0);                           // set PCINT0 (digital input 8) to trigger an interrupt on state change
Serial.begin(9600);
}

void wait(unsigned long i){
 

  if (i > 16380) {
 times=millis()+i/1000;
 while(millis()<times){
    
 //   }
  }
  }
 else{
   times=micros()+i;
      while(micros()<times){
   // Serial.println(micros());
    }
 }
}

ISR(PCINT0_vect){
  
  if(PINB & B00000001){   count++;  }
  
  
  
  
  }

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(count);

if(digitalRead(8)==LOW){
  Serial.println("it is low");}
 digitalWrite(A5,HIGH);

}


