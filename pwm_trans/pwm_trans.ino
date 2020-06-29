
int x=2000;
int flag=0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

DDRD |= (1<<7);
}

void loop() {
  // put your main code here, to run repeatedly:
x= analogRead(A3);
x=map(x,0,1000,1000,2500);
 
 
/*
if(flag==0){
  
  x--;
  if(x==1000){flag=1;}
  }
if(flag==1){
  
  x++;
  if(x==2000){flag=0;}
  }
*/
Serial.println(x); 
//delay(250);
}

