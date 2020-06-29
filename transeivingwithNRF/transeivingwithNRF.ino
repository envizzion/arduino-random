  #include <SPI.h>
  #include "nRF24L01.h"
  #include "RF24.h"
#include "printf.h"
RF24 radio(9,10);

 const uint64_t pipe = 0xE8E8F0F0E1LL;


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

radio.begin();  //starts nrf with whatever config settings have been set.
  radio.setRetries(15,15);// optionally, increase the delay between retries & # of retries
  
  
  radio.openReadingPipe(1,pipe); //opens preset pipe/channel for reading.
  radio.startListening();        // starts listen on above pipe/channel
radio.printDetails(); 


radio.setAutoAck(false);
radio.setDataRate(RF24_250KBPS);




}

void loop() {
  // put your main code here, to run repeatedly:
chk_incoming();
     delay(20);
chk_outgoing();
}


    void chk_incoming() // incoming fuction call...
    {
       
    if ( radio.available() ) // check if a payload came in....
    {
      // Dump the payloads until we've gotten everything
      bool done = false;
      while (!done)
      {
        // Fetch the payload, 
        done = radio.read( button_states, num_button_pins );

        
        printf("Button pressed\n\r"); // for debugging

        // takes recieved button state and toggles the correct led
        int i = num_led_pins;
        while(i--)
        {
          if ( button_states[i] )
          {
            led_states[i] ^= HIGH;
            digitalWrite(led_pins[i],led_states[i]);
          }
         }
        }
      }
}


void chk_outgoing() // chk buttons states to see if we need to send updated info to other unit... 
  {
    int i = num_button_pins;
    bool different = false;
    while(i--)
    {
      uint8_t state = ! digitalRead(button_pins[i]);
      if ( state != button_states[i] )
      {
        different = true;
        button_states[i] = state;
      }
    }
    if (different)// if button has been pressed then.....
    {
      TX_mode(); //calls Tx_mode function
    }
  } 
  


  
  
  void TX_mode()// TX_mode function......
  {
    radio.stopListening(); // must stop listening before you open a writing pipe  
    radio.openWritingPipe(pipe);
    printf("Sending...",button_states,num_button_pins); // for debuggging info
      bool ok = radio.write( button_states, num_button_pins ); // sends data
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

