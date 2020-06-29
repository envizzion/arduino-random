
int s[6]={analogRead(A0),analogRead(A1),analogRead(A2),analogRead(A3),analogRead(A4),analogRead(A5)};
int dig[6];
int sensorMin=1023;
int sensorMax=0;
int sensorValue=0;
int i;
int avg;
int val;
//Left motor
const int L1  = 22;  // Pin 14 of L293
const int L2  = 23;  // Pin 10 of L293
//Right Motor 
const int R1  = 22;  // Pin 14 of L293
const int R2  = 23;  // Pin 10 of L293

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  
 
    
    
  }





  
   
    
 

  

 

void Drive() {

int s[6]={analogRead(A0),analogRead(A1),analogRead(A2),analogRead(A3),analogRead(A4),analogRead(A5)};
String pos="";
pinMode(L1, OUTPUT);
pinMode(L2, OUTPUT);
pinMode(R1, OUTPUT);
pinMode(R2, OUTPUT);


digitalWrite(L1,HIGH);
digitalWrite(L2,LOW);
digitalWrite(R1,HIGH);
digitalWrite(R2,LOW);   
analogWrite(6,0);
analogWrite(7,0);
avg=500;
//ditial convertion
for(int i=0;i<6;i++){
  if(s[i]>=avg){
    pos+='1';
  }
  if(s[i]<avg){
    pos+='0';
    
  }
 
 // if(i==5){
   // Serial.print("\n");
  }
  Serial.println(pos);
// pos=parseInt(dig);


 
  
}












 void caliberate(){

  while ( millis() < 5000) {
  for(int i=0;i<4;i++){
  
 
for(i=0;i<6;i++){
    
    // record the maximum sensor value
   if (s[i] > sensorMax) {
     sensorMax = s[i];
   }

   // record the minimum sensor value
   if (s[i] < sensorMin) {
     sensorMin = s[i];
   }
   
  }
 }
}
avg=(sensorMin+sensorMax)/2;    
}


void loop(){
  caliberate();
  
  
  Drive();
  

  }


