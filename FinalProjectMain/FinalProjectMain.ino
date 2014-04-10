#include <PID_v1.h>
#include <Average.h>
#include <Servo.h> 

double xTarget;                       // Target location for the ball from the computer
double yTarget;                    
double xLoc;                          // Actual loction of the ball from the computer
double yLoc;
double xAngle;                        // Current table angle
double yAngle;
int xResetAngle = 92;                 // Allow for reset angle to be something other than 90
int yResetAngle = 87;  
int isBallOnTable = 1;                // 1 if ball is present 0 if not
Servo xServo;  // create servo object to control the rotation around the x axis
Servo yServo;  // create servo object to control the rotation around the x axis
double pGain = 0;
double iGain = 0;
double dGain = 0;
word data = 0;
int dataPiece = 0;
int BoolNewData = 0;
int maxAngle = 20;
int counter = 0;
double previous;

PID xPID(&xLoc, &xAngle, &xTarget, pGain, iGain, dGain, DIRECT);
PID yPID(&yLoc, &yAngle, &yTarget, pGain, iGain, dGain, REVERSE);


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
  
  xPID.SetMode(AUTOMATIC);
  yPID.SetMode(AUTOMATIC);
  xPID.SetOutputLimits(xResetAngle-maxAngle, xResetAngle+maxAngle);
  yPID.SetOutputLimits(yResetAngle-maxAngle, yResetAngle+maxAngle);
  xPID.SetSampleTime(33);
  yPID.SetSampleTime(33);
}

void loop() {
  //run control structure if the ball is present
  if(BoolNewData) 
  {
    BoolNewData = 0;
    if(isBallOnTable) 
    {
      if (abs(xLoc-xTarget) > 1 && abs(yLoc-yTarget) > 1)
      {
        xPID.SetTunings(pGain, iGain, dGain);
        yPID.SetTunings(pGain, iGain, dGain);
        xPID.Compute();
        yPID.Compute();
        xServo.write(xAngle);  
        yServo.write(yAngle); 
      }
      else
      {
        xServo.write(xResetAngle); // Set table to flat to hold the ball still
        yServo.write(yResetAngle); // Set table to flat to hold the ball still
      }
    }
    else 
    {
      counter++;
      if (counter > 30) {
        counter = 0;
        xServo.write(xResetAngle); // Set table to flat if ball is not present
        yServo.write(yResetAngle); // Set table to flat if ball is not present
        xAngle = xResetAngle;
        yAngle = yResetAngle;
      }
    }
  }
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
  } 
}
