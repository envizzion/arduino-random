int PIN_CLK = 10;
 
void setup()
{
  pinMode(PIN_CLK, OUTPUT);
  digitalWrite(PIN_CLK, LOW);
}
 
void loop()
{
  digitalWrite(PIN_CLK, HIGH);
  //delay(2000);
  delay(2);
  digitalWrite(PIN_CLK, LOW);
  delay(2);
 //delay(2000);
}
