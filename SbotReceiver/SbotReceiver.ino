  #include <SPI.h>
  #include "nRF24L01.h"
  #include "RF24.h"
  #include "printf.h"
RF24 radio(9,10);

 const uint64_t pipe = 0xE8E8F0F0E1LL;
//0-RED ,1-GREEN,2-BLUE
uint8_t colors[] = { 0,0,0,0 };
uint8_t complete[] = { 1,1,1};
boolean wait_for_col=true;
boolean wait_for_shoot=false;

void setup() {
  // put your setup code here, to run once:
  printf_begin();
Serial.begin(9600);
Serial.println("run1");
radio.begin();  //starts nrf with whatever config settings have been set.
 // radio.setRetries(15,15);// optionally, increase the delay between retries & # of retries
Serial.println("run2");  
  
radio.openReadingPipe(1,pipe); //opens preset pipe/channel for reading.
radio.startListening();        // starts listen on above pipe/channel

radio.setAutoAck(false);
radio.setDataRate(RF24_250KBPS);

radio.printDetails(); 
Serial.println("run3"); 


pinMode(A0,OUTPUT);//color
pinMode(A1,OUTPUT);//distance



}

void TX_mode()// TX_mode function......
  {
    radio.stopListening(); // must stop listening before you open a writing pipe  
    radio.openWritingPipe(pipe);
    printf("Sending..."); // for debuggging info
      bool ok = radio.write( complete, 3 ); // sends data
      if (ok) {              // checks if data was propperly received..
        printf("Ok\n\r");    // for debugging
        
        
        radio.openReadingPipe(1,pipe); // closes writing pipe and opens Reading pipe
        radio.startListening();        // sets listening mode so were receiving and we start all over again from the beginning...
        delay(200);
      }
      else {
        printf("Failed\n\r"); // for debugging " lets you know other did not received transmittion "
      } 

  }
    void chk_incoming() {
       
    if ( radio.available() ) // check if a payload came in....
    {
      // Dump the payloads until we've gotten everything
     // Fetch the payload, 
      printf("reading ongoing"); // for debugging

      radio.read( colors,4);
      
         int i = 4;
        while(i--)
        {
          if (colors[i])
          {
            //led_states[i] ^= HIGH;
            //digitalWrite(led_pins[i],led_states[i]);
            Serial.println(i);
            wait_for_col=false;
            wait_for_shoot=true;
           
            break;
          }
         }
        }
      }






void loop() {
unsigned long tim=millis();
  // put your main code here, to run repeatedly:
while(wait_for_col){  
chk_incoming();
if((tim+3000)<millis()){break;}
}
    // delay(20);

tim=millis();    
while(wait_for_shoot){  



while(!digitalRead(5)){
  
  if(colors[0])analogWrite(A0,300);
else if(colors[1])analogWrite(A0,600);
else if(colors[2])analogWrite(A0,900);
else if(colors[3]>1){
  float x=colors[3];
  analogWrite(A1,x*10);
}
  
  }

analogWrite(A0,10);
analogWrite(A1,10);  
wait_for_shoot=false;  



}
//transmit for 3 seconds
unsigned long  ttme=millis();

while(ttme+3000<millis()){
TX_mode();
}
}



  


  
  
  

