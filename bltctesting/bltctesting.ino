  // put your setup code here, to run once:
int AA1=13;
int AA2=12;
int BB1=11;
int BB2=10;
int CC1=4;
int CC2=3;

void setup() {
Serial.begin(9600);
  pinMode(AA1,OUTPUT);
  pinMode(AA2,OUTPUT);
  pinMode(BB1,OUTPUT);
  pinMode(BB2,OUTPUT);
  pinMode(CC1,OUTPUT);
  pinMode(CC2,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(AA1,HIGH);
      digitalWrite(AA2,LOW);
      digitalWrite(BB1,HIGH);
      digitalWrite(CC2,LOW);
      digitalWrite(BB2,LOW);
      digitalWrite(CC1,HIGH);
      delay(2000);
     digitalWrite(AA1,LOW);
      digitalWrite(AA2,LOW);
      digitalWrite(BB1,LOW);
      digitalWrite(CC2,LOW);
      digitalWrite(BB2,LOW);
      digitalWrite(CC1,LOW);
       delay(2000);
        
}
