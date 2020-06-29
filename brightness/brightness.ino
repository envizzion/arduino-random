const int ledPin=3;
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
for(int i=0;i<=255;i+=5){
  analogWrite(ledPin,i);
  delay(50);
  if(i==255){
    while(i>0){
      analogWrite(ledPin,i);
      i-=5;
      delay(50);
    }
    
  }
}
}
