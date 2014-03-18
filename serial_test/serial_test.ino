byte inByte = 0;
int led = 13;

void setup()
{
  // Initialize serial
  Serial.begin(115200);
  
  // Initialize the LED pin as an output
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
}

void loop()
{
  if (Serial.available())
  {
    inByte = Serial.read();
    if (inByte == 150)
    {
      digitalWrite(led, HIGH);
    }
    Serial.write(inByte);
  }
}
