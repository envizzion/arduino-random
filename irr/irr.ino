void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
int s[6]={analogRead(A0),analogRead(A1),analogRead(A2),analogRead(A3),analogRead(A4),analogRead(A5)};

for(int i=0;i<6;i++){
  Serial.println(s[i]);
}
Serial.println("");
delay(1000);
}
