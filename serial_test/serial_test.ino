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
    data = readInt();
    
    // Matlab is sending an 8, verify it was received correctly
    if (data = 8)
    {
      digitalWrite(led, HIGH);
    }
    
    writeInt(data);
  }
}

int readInt()
{
  char buf[] = {0,0};
  Serial.readBytes(buf,2);
  return word(buf[1],buf[0]); 
}

void writeInt(int data)
{
    // Write low, then high byte
    Serial.write(lowByte(data));
    Serial.write(highByte(data));
}
