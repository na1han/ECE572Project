char buf[] = {0,0};
word data = 0;
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
  if (Serial.available() > 1)
  {
    // Read two bytes into buf.
    // buf[0] is the low byte
    // buf[1] is the high byte
    Serial.readBytes(buf,2);
    data = word(buf[1],buf[0]);
    
    // Matlab is sending an 8, verify it was received correctly
    if (data = 8)
    {
      digitalWrite(led, HIGH);
    }
    
    // Write low, then high byte
    Serial.write(buf[0]);
    Serial.write(buf[1]);
  }
}
