
void setup(){
 pinMode(6,OUTPUT);
pinMode(7,OUTPUT);
pinMode(22,OUTPUT);
pinMode(23,OUTPUT);
pinMode(24,OUTPUT);
pinMode(25,OUTPUT);
 
}

void loop(){

 digitalWrite(22,LOW);
 digitalWrite(23,LOW);
 digitalWrite(24,LOW);
 digitalWrite(25,LOW);
 
for(int i=0;i<=255;i+=10){
  analogWrite(7,255);
  analogWrite(6,255);
  delay(50);
}
  
}

