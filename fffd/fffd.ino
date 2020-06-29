int X=13
;
int y=12;

void setup() {
  // put your setup code here, to run once:
pinMode(X,OUTPUT);
pinMode(y,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(X,HIGH);
digitalWrite(y,HIGH);
delay(2000);
digitalWrite(X,LOW);
digitalWrite(y,LOW);
delay(2000);
}
