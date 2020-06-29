
int sensorMin=1023;
int sensorMax=0;
int sensorValue=0;
int i;
int avg;
int val;
const int L1 =22  ;// Pin 14 of L293
const int L2 =23  ;// Pin 10 of L293
const int Lc =10  ;//pwm control1
//Motor B
const int R1 =24  ;// Pin  7 of L293
const int R2 =25  ;// Pin  2 of L293
const int Rc =11  ;// pwm contol2
 int sAnalog[6]={analogRead(A0),analogRead(A1),analogRead(A2),analogRead(A3),analogRead(A4),analogRead(A5)};
int sDigital[6];
int Lspeed=100;
int Rspeed=100;
void setup() {
  // put your setup code here, to run once:
pinMode(L1, OUTPUT);
pinMode(L2, OUTPUT);
pinMode(R1, OUTPUT);
pinMode(R2, OUTPUT);
pinMode(Lc,OUTPUT);
pinMode(Rc,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
analogWrite(Lc,Lspeed);
  digitalWrite(L1,LOW);
  digitalWrite(L2,LOW);
  analogWrite(Lc,100);
  digitalWrite(R1,HIGH);
  digitalWrite(R2,LOW);
}


