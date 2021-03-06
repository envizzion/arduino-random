
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
const int Lc = 6;
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
const int node=105;
const int turn=106;
int turnTime=1000;
long int timer=0;
long int interval=550;  
int juncCount=0;
int stopMapping=500;
const int up=1;
const int down=2;
const int finish=3;
String dirArray;
String finalDir;
int shortPath=500;
int juncNum=0;
void setup() {
  // initialize serial communication at 9600 bits per second:
//Serial.begin(9600)
 pinMode(L1, OUTPUT);
pinMode(L2, OUTPUT);
pinMode(R1, OUTPUT);
pinMode(R2, OUTPUT);

    
    
  }




void steer(int left,int right){
  analogWrite(Lc,left);
  analogWrite(Rc,right*1.1);
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


void    shortstPath(){
  String finDir=dirArray;
  
  do{
    finalDir=finDir;
    finDir.replace("LTR","T");
    finDir.replace("LTU","R");
    finDir.replace("RTL","T");
    finDir.replace("UTL","R");
    finDir.replace("UTU","T");
    finDir.replace("LTL","U");
    
    }while(finDir.length()!=finalDir.length());
  }

void mapPath(const int juncType){
  
  switch(juncType){

    case left:{dirArray+='L';}
    break;
    case right:{dirArray+='R';}
    break;
    case up:{dirArray+='U';}
    break;
    case down:{dirArray+='D';}
    break;
    case turn:{dirArray+='T';}
    break;
    case finish:{shortstPath();stopMapping=1000;}
    break;
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
  //Serial.println(posStr);
  
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

 bool findJunc(){
 if(pos()!=111111){
  switch (pos()){
  case 221122: case 211222: case 222112: case 221222: case 222122:{   
  return true;
  }
  break;
  }
 }
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
            steer(mid-=2,fas);
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
             steer(fas,mid-=2);
             //delay(20);
             
             //}
            //else{cont=0;}
          //} 
        }
        break;

        case left:{
         
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
            
               int cont=1; 
               digitalWrite(L1,HIGH);
               digitalWrite(L2,LOW);
          while(cont==1){

            if(pos()!=221122){
            steer(med,med);}
            
            else{cont=0;}
          }
         
         }
         break;
 
    }
 
  
  
  //}   
    
 //}
} 

void getDir(){

  switch(finalDir.charAt(i)){

    case 'L':{path(left);}
    break;
     case 'R':{path(right);}
    break;
     case 'U':{path(forward);}
    break;
     case 'D':{path(rotateLeft);}
    break;
    
    }
  
  i++;
  
  
  }

  

 


void mapDrive() {

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
   
   case 222111: case 221111: case 211111:{
      getSet();
      
      if(findJunc()==true){
      mapPath(up);
      path(forward);
      }
      else{path(right);}
    }
     break;

    case 111222: case 111122: case 111112:{
       getSet();
       if(findJunc()==true){
        mapPath(left);
       }
       
       path(left);  
    }
    break;
    
    case 111111:{
     getSet();
     

     if(pos==111111){
      mapPath(finish);
      stopMapping=1000;
      path(stay);
      }
     else{
      mapPath(left);
      path(left);     
      }
    
    }
    break;
    
    case 222222:{
     getSet();
     mapPath(turn);
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


void driveShortPath() {

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
   
   case 222111: case 221111: case 211111:{
      getSet();
      
      if(findJunc()==true){
      getDir();
      }
      else{path(right);}
    }
     break;

    case 111222: case 111122: case 111112:{
       getSet();
       if(findJunc()==true){
        getDir();
       }
       
       path(left);  
    }
    break;
    
    case 111111:{
     getSet();
     

     if(pos==111111){
      
      stopMapping=1000;
      path(stay);
      }
     else{
      getDir();     
      }
    
    }
    break;
    
    case 222222:{
     getSet();
     
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
 // int s[6]={analogRead(A0),analogRead(A1),analogRead(A2),analogRead(A3),analogRead(A4),analogRead(A5)};
  
// Serial.println(pos());
 while(stopMapping!=1000){
 mapDrive();
 /* for(int i=0;i<=5;i++){
    Serial.println(s[i]);
    if(i==5){Serial.println("\n");
    
    }
}
  }*/
 
  //delay(1000);
}

delay(7000);

while(shortPath!=1000){
  driveShortPath();
  
  
  }
}


