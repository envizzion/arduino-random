int Distance = 0;  // Record the number of steps we've taken

void setup() {               
  pinMode(11, OUTPUT);    
  pinMode(10, OUTPUT);
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  
}

void loop() {
  digitalWrite(10, HIGH);
  delayMicroseconds(100);         
  digitalWrite(10, LOW);
  delayMicroseconds(100);
  Distance = Distance + 1;   // record this step
 
  // Check to see if we are at the end of our move
  if (Distance == 3600)
  {
    // We are! Reverse direction (invert DIR signal)
    if (digitalRead(11) == LOW)
    {
      digitalWrite(11, HIGH);
    }
    else
    {
      digitalWrite(11, LOW);
    }
    // Reset our distance back to zero since we're
    // starting a new move
    Distance = 0;
    // Now pause for half a second
    delay(500);
  }
}
