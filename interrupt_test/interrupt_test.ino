
unint8_t count=0;

void timer1_Init(){
  TCCR
  
  }


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
 
  int x;
  // put your main code here, to run repeatedly:
x=analogRead(A0);
analogWrite(3,x/4);
 Serial.println(x);
 
}

