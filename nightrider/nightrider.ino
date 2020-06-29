

 int arr[4]={3,5,6,9};
 int maxB=250;
 int start=1;

 void bright(int x){
  
  for(int k=0;k<=maxB;k+=10){
  analogWrite(arr[x],k);
  analogWrite(arr[x-1],maxB-k);
  delay(5);
  }
 }

 void brightRed(int y){
 for(int k=0;k<=maxB;k+=10){
  analogWrite(arr[y],maxB-k);
  analogWrite(arr[y-1],k);
  delay(5);
 }
 
 }
void setup() {
  // put your setup code here, to run once:



}

void loop() {
  // put your main code here, to run repeatedly:

for(int i=0;i<4;i++){
  
  if(i==0&& start==1){
  for(int j=0;j<=maxB;j+=10){
    analogWrite(arr[i],j);
    delay(50);
  }
  }
  if(i>0){
   bright(i);
  }
  

  if(i==3){
   for(i=3;i>=0;i--){
   if(i>0){
    
    brightRed(i);
   if(i==0){
    for(int m=maxB;m>=0;m-=10){
      analogWrite(arr[i],m);
    }
   }
   }
      
    }
    start=0;
  }
}
}


