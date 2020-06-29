void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  int s[6]={analogRead(A0),analogRead(A1),analogRead(A2),analogRead(A3),analogRead(A4),analogRead(A5)};
  // put your main code here, to run repeatedly:
 for(int i=0;i<=5;i++){
    Serial.println(s[i]);
    if(i==5){Serial.println("\n");
    
    }

  }
  delay(1000);
}
