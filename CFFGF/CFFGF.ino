int AA1=13;
int AA2=12;
int BB1=11;
int BB2=10;
int CC1=9;
int CC2=8;



int emfA=A1;
int emfB=A2;
int emfC=A3;

int emA = 0;
      int emB =0;
      int emC =0;


//int led = 6;
unsigned long stepLength = 20000;
int minStepLength =700;
int steps =2;

void setup() {
Serial.begin(250000);
  pinMode(AA1,OUTPUT);
  pinMode(AA2,OUTPUT);
  pinMode(BB1,OUTPUT);
  pinMode(BB2,OUTPUT);
  pinMode(CC1,OUTPUT);
  pinMode(CC2,OUTPUT);
  pinMode(emfA,INPUT);
  pinMode(emfB,INPUT);
  pinMode(emfC,INPUT);


  digitalWrite(AA2,LOW);
      digitalWrite(BB1,LOW);
      digitalWrite(BB2,LOW);
      digitalWrite(CC1,LOW);
      digitalWrite(CC2,LOW);
      digitalWrite(AA1,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(AA1,HIGH);
//digitalWrite(AA2,LOW);
digitalWrite(CC2,LOW);
//delay(200);

emA = analogRead(emfA);
//minStepLength = analogRead(A4);
        emB = analogRead(emfB);
        emC = analogRead(emfC);
Serial.print(emA);Serial.print("  ");Serial.print(emB);Serial.print("  ");Serial.print(emC);Serial.print("  ");Serial.print("H");Serial.println("");

delay(2000);


digitalWrite(AA1,LOW);
//digitalWrite(AA2,HIGH);
digitalWrite(CC2,HIGH);
//delay(200);
emA = analogRead(emfA);
//minStepLength = analogRead(A4);
        emB = analogRead(emfB);
        emC = analogRead(emfC);
Serial.print(emA);Serial.print("  ");Serial.print(emB);Serial.print("  ");Serial.print(emC);Serial.print("  ");Serial.print("L");Serial.println("");




delay(2000);
}

