const int R1  = 24;  // Pin 14 of L293
const int R2  = 25;  // Pin 10 of L2
const int Rc = 8;
//Right Motor 
const int L1  = 26;  // Pin 14 of L293
const int L2  = 27;  // Pin 10 of L293
const int Lc = 6;



  


void setup() {
  // put your setup code here, to run once:
pinMode(L1, OUTPUT);
pinMode(L2, OUTPUT);
pinMode(R1, OUTPUT);
pinMode(R2, OUTPUT);
analogWrite(6,0);
analogWrite(7,0);

}

void loop() {
  digitalWrite(L1,HIGH);
digitalWrite(L2,LOW);
digitalWrite(R1,HIGH);
digitalWrite(R2,LOW);
  // put your main code here, to run repeatedly:
analogWrite(Rc,130);
analogWrite(Lc,130);


}
