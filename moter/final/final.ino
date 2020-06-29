
int s[6]={analogRead(A0),analogRead(A1),analogRead(A2),analogRead(A3),analogRead(A4),analogRead(A5)};
int dig[6];
int sensorMin=1023;
int sensorMax=0;
int sensorValue=0;
int i;
int avg;
int val;
//Left motor
const int R1  = 24;  // Pin 14 of L293
const int R2  = 25;  // Pin 10 of L2
const int Rc = 8;
//Right Motor 
const int L1  = 26;  // Pin 14 of L293
const int L2  = 27;  // Pin 10 of L293
const int Lc = 3;
const int fast=210;
const int med=150;
int high_med=190;
int low=130;
int wait=0;
const int forward=98;
const int stay=99;
const int left=100;
const int right=101;
const int slightLeft=102;
const int slightRight=103;
const int rotateLeft=104;
int turnTime=1000;
long int timer=0;
long int interval=550;  
void setup() {
  // initialize serial communication at 9600 bits per second:
//Serial.begin(9600);
 pinMode(L1, OUTPUT);
pinMode(L2, OUTPUT);
pinMode(R1, OUTPUT);
pinMode(R2, OUTPUT);

    
    
  }




void steer(int left,int right){
  analogWrite(Lc,left);
  analogWrite(Rc,right);
 // delay(50);

}

void getSet(){
 long int nxtTime=0;
 timer=millis();
 nxtTime=timer+interval;
 while(nxtTime>=timer){
  timer=millis();
  steer(fast,fast); 
 }
}



 long int pos() {

int s[6]={analogRead(A0),analogRead(A1),analogRead(A2),analogRead(A3),analogRead(A4),analogRead(A5)};
String posStr="";
long int pos;
 


//ditial convertion
for(int i=0;i<6;i++){
  if(s[i]>=avg){
    posStr+='1';
  }
  if(s[i]<avg){
    posStr+='0';
    
  }
 
 // if(i==5){
   // Serial.print("\n");
  }
//  Serial.println(posStr);
  
// pos=parseInt(dig);



 

//string to integer convertion
if(posStr.equals("000000")){
 pos=222222;
}
else if(posStr.equals("001100")){
 pos=221122;
}
else if(posStr.equals("001000")){
 pos=221222;
}
else if(posStr.equals("000100")){
 pos=222122;
}
else if(posStr.equals("011110")){
 pos=211112;
}
else if(posStr.equals("111111")){
 pos=111111;
}
else if(posStr.equals("000111")){
 pos=222111;
}
else if(posStr.equals("111000")){
   pos=111222;
}
else if(posStr.equals("111100")){
 pos=111122;
}
else if(posStr.equals("111111")){
 pos=111111;
}
else if(posStr.equals("001111")){
 pos=221111;
}
else if(posStr.equals("011000")){
  pos=211222;
}
else if(posStr.equals("000110")){
  pos=222112;
}
else if(posStr.equals("111110")){
  pos=111112;
}
else if(posStr.equals("011111")){
  pos=211111;
}
else if(posStr.equals("111110")){
  pos=222221;
}
else if(posStr.equals("100000")){
  pos=122222;
}
else if(posStr.equals("0000001")){
  pos=222221;
}
else{
  pos=2;
}

return pos;
//Serial.println(pos);
}

 




void path(long int dir){

 /*if(dir==forward && poss==221122||dir==forward && poss==211112){

   steer(fast,fast);
 }
 else if (dir==stay){
  steer(wait,wait);
 }
 else{
  
  //while(pos()!=221122||pos()!=211112){ */
 

digitalWrite(L1,LOW);
digitalWrite(L2,HIGH);
digitalWrite(R1,LOW);
digitalWrite(R2,HIGH);  
analogWrite(Lc,0);
analogWrite(Rc,0);
int mid=med;
int fas=fast;
    switch(dir){
        case forward:{
         steer(med,med);
             
        }
        break;
       
        case slightLeft:{
          //int cont=1;
           //while(cont==1){
           //if(pos()!=221122){ 
            //if(fas<240){fas+=1;}
            steer(mid,fas);
             //delay(20);
            //}
           //else{cont=0;}
          //} 
        }
        break;

        case slightRight:{
          //int cont=1;
            //while(cont==1){
           //if(pos()!=221122){  
             //if(fas<240){fas+=1;}
             steer(fas,mid);
             //delay(20);
             
             //}
            //else{cont=0;}
          //} 
        }
        break;

        case left:{
          getSet();
         digitalWrite(L1,HIGH);
          digitalWrite(L2,LOW);  
          int cont=1; 
          
          while(cont==1){
            if(pos()!=221122){
            steer(med,med);}
           else{cont=0;}
          }
        } 
        
        break;
        
       case right:{
        getSet();
        int cont=1; 
        digitalWrite(R1,HIGH);
        digitalWrite(R2,LOW);
       
          while(cont==1){
            if(pos()!=221122) {steer(med,med);}
            
            else{cont=0;}
          }
           
          
        }
        break;
     
        case rotateLeft :{
               getSet();
               if(pos()!=222222){
                path(forward);
               }
               int cont=1; 
               digitalWrite(L1,HIGH);
               digitalWrite(L2,LOW);
          while(cont==1){

            if(pos()!=222112){
            steer(med,med);}
            
            else{cont=0;}
          }
         
         }
         break;
 
    } 
 
  
  
  //}   
    
 //}
} 

  

 


void Drive() {

switch(pos()){

   case 221122:{
    path(forward);
   }
   break;
   case 222112:{
     path(slightRight);
   }
   break;
   
   case 211222:{
     path(slightLeft);
   }
   break;
   
   case 222111:{
    
      path(right);
      
    }
     break;
      
    
     
     
     case 221111:{
      path(right);
       
    }
     break;
      case 211111:{
      path(right);
       
    }
     break;

    case 111222:{
       path(left);
       
    }
    break;

    case 111122:{
       path(left);
         
    }
    break;
    
    case 111112:{
       path(left);
         
    }
    break;
    
    case 111111:{
     
     path(left);
    
    }
    break;
   
    case 222222:{
      
     path(rotateLeft); 
    }
    break;
    case 222122:{
      
     path(slightRight); 
    }
    break;
    case 221222:{
      
     path(slightLeft); 
    }
    break;
} 
}
  













 void caliberate(){
  int s[6]={analogRead(A0),analogRead(A1),analogRead(A2),analogRead(A3),analogRead(A4),analogRead(A5)};

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
//Serial.println(avg);
}




void loop(){
  caliberate();
 //int s[6]={analogRead(A0),analogRead(A1),analogRead(A2),analogRead(A3),analogRead(A4),analogRead(A5)};
  //Serial.println(pos());
 
 Drive();
/* for(int i=0;i<=5;i++){
    Serial.println(s[i]);
    if(i==5){Serial.println("\n");
    
    }

  }
 
  delay(1000);*/
}



