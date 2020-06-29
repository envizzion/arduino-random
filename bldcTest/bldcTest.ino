
int AA1=22;
int AA2=24;
int BB1=26;
int BB2=28;
int CC1=30;
int CC2=32;

int enable=2;
int emfA=A2;
int emfB=A3;
int emfC=A4;
void setup() {
  // put your setup code here, to run once:
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
digitalWrite(AA2,LOW);
      digitalWrite(BB1,LOW);
      digitalWrite(CC1,LOW);
      digitalWrite(CC2,LOW);
      digitalWrite(AA1,HIGH);
      digitalWrite(BB2,HIGH);
      delay(100);
}
