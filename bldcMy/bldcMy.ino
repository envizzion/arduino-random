int p1 = 13;
int  p2 = 12;
int p3 = 8;
int led = 12;
unsigned long stepLength = 30000;
int minStepLength = 500;
int steps =5 ;

void setup() {
  pinMode(led, OUTPUT);  
  pinMode(p1, OUTPUT);
  pinMode(p2, OUTPUT);
  pinMode(p3, OUTPUT);
  
}

void loop() {
  digitalWrite(p1,HIGH);
  digitalWrite(p2,LOW);
  digitalWrite(p3,LOW);
  
  }
