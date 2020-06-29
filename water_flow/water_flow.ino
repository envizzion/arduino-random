volatile int flow_frequency; // Measures flow sensor pulses
float l_hour; // Calculated litres/hour
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
float ltr=0;
float rate=4.5;
#include<Servo.h>
Servo valve;
int pos=45;

void flow () // Interrupt function
{
   flow_frequency++;
}
void setup()
{
  
  valve.attach(9);
   pinMode(flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   Serial.begin(9600);
   attachInterrupt(0, flow, RISING); // Setup Interrupt
   sei(); // Enable interrupts
   currentTime = millis();
   cloopTime = currentTime;
   valve.write(0);
   delay(500);
   
}
void loop ()
{
   currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
      cloopTime = currentTime; // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_hour = (flow_frequency / 9.51); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flow_frequency = 0; // Reset Counter
      Serial.print(l_hour); // Print litres/hour
      Serial.println(" L/min");
      
      ltr+=l_hour*1/60;
      float tim=cloopTime/1000;
     Serial.print(ltr);
     Serial.print(" litres  ");
     Serial.println(tim);
      
   
     /* while(millis()>1000){
           if(l_hour<rate){
        valve.write(pos-=5);
      }
      if(l_hour>rate){
        valve.write(pos+=5);
        }
        
       }

      if(millis()>1000&& millis()<2000){
       while(millis()>2000){
          if(l_hour<rate){
            valve.write(pos-=5);
          }
          if(l_hour>rate){
            valve.write(pos+=5);
          }
        
       }
      }*/
      
     
      
       if(l_hour>rate-0.1 && l_hour<rate+0.1){
         
       }
      else{ 
      
      if(l_hour>rate){
        if(pos<80){
        valve.write(pos+=5);
        }
        }
      
     else if(l_hour<rate){
       if(pos>5){
        valve.write(pos-=5);
       }
        }
      }
   }
   
}

