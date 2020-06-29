

long int timer;

long int interval=1000;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
 
}
void getSet(){
  long int nxtTime=0;
 timer=millis();
  nxtTime=timer+interval;
  while(nxtTime>=timer){

     timer=millis();
  }
      Serial.println("timeRunning");
     Serial.println(nxtTime);
      Serial.println(timer);   
     
  }
void loop() {
  // put your main code here, to run repeatedly:
getSet();
delay(3000);

//Serial.println(timer);
}
