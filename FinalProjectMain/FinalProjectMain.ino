
#include <Average.h>
#include <Servo.h> 

int xTarget;                     // Target location for the ball from the computer
int yTarget;                    
int xLoc;                        // Actual loction of the ball from the computer
int yLoc;
int xAngle;                      // Current table angle
int yAngle;
int xResetAngle = 92;                 // Allow for reset angle to be something other than 90
int yResetAngle = 87;  
int isBallOnTable = 1;                // 1 if ball is present 0 if not
Servo xServo;  // create servo object to control the rotation around the x axis
Servo yServo;  // create servo object to control the rotation around the x axis
float pGain = 0;
float iGain = 0;
float dGain = 0;
int previousError;
float dBuffer[] = {0, 0, 0, 0};
float iAccum = 0;
word data = 0;
int dataPiece = 0;
int BoolRX = 0;
int BoolNewData = 0;
int maxAngle = 20;


void setup() {
  // initialize serial:
  Serial.begin(115200);
  xServo.attach(9);  // attaches the servo on pin 9 to the servo object
  yServo.attach(8);  // attaches the servo on pin 8 to the servo object
  xAngle = xResetAngle; 
  yAngle = yResetAngle;
  xServo.write(xResetAngle); // Set table to flat if ball is not present
  yServo.write(yResetAngle); // Set table to flat if ball is not present
  isBallOnTable = 0;
}

void loop() {
    //writeDisplay();
    //run control structure if the ball is present
    if(BoolNewData) {
      BoolNewData = 0;
      if(isBallOnTable) {
        xAngle = xAngle - PID(xLoc - xTarget);
        yAngle = yAngle + PID(yLoc - yTarget);
        xServo.write(constrain(xAngle, xResetAngle-maxAngle, xResetAngle+maxAngle)); // Only allowing 15 degrees of correction for now 
        yServo.write(constrain(yAngle, yResetAngle-maxAngle, yResetAngle+maxAngle)); // Only allowing 15 degrees of correction for now
      }
//      else {
//        xServo.write(xResetAngle); // Set table to flat if ball is not present
//        yServo.write(yResetAngle); // Set table to flat if ball is not present
//        xAngle = xResetAngle;
//        yAngle = yResetAngle;
//      }
    }
}

int PID(int error) {
  float pWeight = error*pGain;
  iAccum = constrain(iAccum + error, -1000, 1000);
  float iWeight = iAccum*iGain;
//  for(int i = 0; i<3; i++) {
//    dBuffer[i] = dBuffer[i+1];
//  }
//  dBuffer[3] = error;
//  int average = 0;
//  for(int i = 0; i<4; i++) {
//    average = average + dBuffer[i];
//  }
//  average = average/4;
  float dWeight = (error - previousError)*dGain;
  previousError = error;
  return (int)round(pWeight + iWeight + dWeight);
}

void serialEvent()
{
  if (Serial.available())
  {
    char buf[] = {0,0};
    Serial.readBytes(buf,2);
    data =  word(buf[1],buf[0]);
    if (data == 5555)
      dataPiece = 0;
    if (dataPiece == 1)
      xTarget = data;
    if (dataPiece == 2)
      yTarget = data;
    if (dataPiece == 3)
      xLoc = data;
    if (dataPiece == 4)
      yLoc = data;
    if (dataPiece == 5)
      isBallOnTable = data;
    if (dataPiece == 6)
      pGain = (float)data/1000;
    if (dataPiece == 7)
      iGain = (float)data/1000;
    if (dataPiece == 8)
    {
      dGain = (float)data/1000;
      BoolNewData = 1;
    }
      
    dataPiece = dataPiece+1;
    writeInt(data);
  }
  
}

void writeInt(int data)
{
    // Write low, then high byte
    if(BoolRX)
    {
      Serial.write(lowByte(data));
      Serial.write(highByte(data));
    }
}

