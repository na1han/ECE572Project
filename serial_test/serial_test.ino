word data = 0;
int led = 13;
int dataPiece = 0;
int xTarget;                     // Target location for the ball from the computer
int yTarget;                    
int xLoc;                        // Actual loction of the ball from the computer
int yLoc; 
int isBallOnTable; 


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
//  if (Serial.available() > 1)
//  {
//    data = readInt();
//    
//    // Matlab is sending an 8, verify it was received correctly
//    if (data == 8)
//    {
//      digitalWrite(led, HIGH);
//    }
//    
//    writeInt(data);
//  }
}

void serialEvent()
{
  if (Serial.available())
  {
    char buf[] = {0,0};
    Serial.readBytes(buf,2);
    data =  word(buf[1],buf[0]);
    if (data == 5555)
    {
      dataPiece = 0;
    }
    if (dataPiece = 1)
      xTarget = data;
    if (dataPiece = 2)
      yTarget = data;
    if (dataPiece = 3)
      xLoc = data;
    if (dataPiece = 4)
      yLoc = data;
    if (dataPiece = 5)
      isBallOnTable = data;
      
    dataPiece = dataPiece+1;
    writeInt(data);
  }
  
}

void writeInt(int data)
{
    // Write low, then high byte
    Serial.write(lowByte(data));
    Serial.write(highByte(data));
}
