  #include <SPI.h>
  #include "nRF24L01.h"
  #include "RF24.h"
#include "printf.h"
RF24 radio(9,10);

 const uint64_t pipe = 0xE8E8F0F0E1LL;
//0-RED ,1-GREEN,2-BLUE
 uint8_t colors[] = { 0,0,0,0};
 uint8_t complete[] = { 0,0,0};

boolean shoot_success=false;
boolean color_transmitted=false;
boolean wait_for_reply=true;
boolean wait_for_col=true;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
 printf_begin();
radio.begin();  //starts nrf with whatever config settings have been set.
//radio.setRetries(15,15);// optionally, increase the delay between retries & # of retries
  
  
pinMode(6,HIGH);
radio.printDetails(); 

  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipe);




}




void TX_mode()// TX_mode function......
  {
   
    printf("Sending..."); // for debuggging info
      bool ok = radio.write( colors, 3 ); // sends data
      if (ok) {              // checks if data was propperly received..
        printf("Ok\n\r");    // for debugging
        
        if(colors[0] || colors[1] ||colors[2] || colors[3]>1){color_transmitted=true;} 
       for(int i=0 ;i<3;i++){colors[i]=0;}
        delay(200);
        
      }
      else {
        printf("Failed\n\r"); // for debugging " lets you know other did not received transmittion "
      } 

  }



    
    void await_response() {
       
     radio.openReadingPipe(1,pipe); // closes writing pipe and opens Reading pipe
        radio.startListening();        // sets listening mode so were receiving and we start all over again from the beginning...
   printf("awaiting response");
    while(!shoot_success){
     if(radio.available()){
         
      printf("reading ongoing");
        radio.read(complete,3); 

      for(int i=0;i<3;i++){
        Serial.print(complete[i]);Serial.print(" ");
        if(complete[i])digitalWrite(6,HIGH);
        }
        Serial.println("");
       shoot_success=true;
       
      }
    } 
    radio.stopListening(); // must stop listening before you open a writing pipe  
    radio.openWritingPipe(pipe);
    }






void loop() {
  // put your main code here, to run repeatedly:
while(wait_for_col){  
//check for any colors from base
//}
while(!color_transmitted)TX_mode();

await_response();

 shoot_success=false;
 color_transmitted=false;
 wait_for_reply=true; 
  
  }





  


  
  
  

