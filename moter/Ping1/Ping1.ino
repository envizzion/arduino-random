#include <NewPing.h>

#include<Servo.h>


#define TRIGGER_PIN  50                // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN  51                  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

Servo myservo;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins .
int pos=0;
void setup() {
  Serial.begin(9600); // Open serial monitor at 9600 baud to see ping results.
  myservo.attach(6);
}

void loop() {
  delay(50);       // Wait 50ms between pings ( 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  Serial.print("Ping");
  Serial.print(uS / US_ROUNDTRIP_CM); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
  Serial.println("cm"); 
  unsigned int dist= uS/US_ROUNDTRIP_CM;

  if(dist<40){
    
for (pos = 0; pos <= 0; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(35);    // waits 15ms for the servo to reach the position
  }
     
    
  }
   
    
    
  

  
}
