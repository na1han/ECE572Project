#include <Average.h>
#include <Servo.h> 
#include "LedControl.h"

/***** These pin numbers will probably not work with your hardware *****
pin 12 is connected to the DataIn 
pin 11 is connected to the CLK 
pin 10 is connected to LOAD  */ 
LedControl lc=LedControl(12,11,10,2);
int xTarget;                     // Target location for the ball from the computer
int yTarget;                    
int xLoc;                        // Actual loction of the ball from the computer
int yLoc;
int xAngle;                      // Current table angle
int yAngle;
int xResetAngle;                 // Allow for reset angle to be something other than 90
int yResetAngle;  
int isBallOnTable = 1;                // 1 if ball is present 0 if not
Servo xServo;  // create servo object to control the rotation around the x axis
Servo yServo;  // create servo object to control the rotation around the x axis
float pGain = .01;
float iGain = .01;
float dGain = 1;
float dBuffer[] = {0, 0, 0, 0};
float iAccum = 0;
word data = 0;
int led = 13;
int dataPiece = 0;
int BoolRX = 0;


void setup() {
  // initialize serial:
  Serial.begin(115200);
  xServo.attach(9);  // attaches the servo on pin 9 to the servo object
  yServo.attach(8);  // attaches the servo on pin 8 to the servo object
  xResetAngle = 90;
  yResetAngle = 85;
  xAngle = xResetAngle; 
  yAngle = yResetAngle;
  xServo.write(xResetAngle); // Set table to flat if ball is not present
  yServo.write(yResetAngle); // Set table to flat if ball is not present
  isBallOnTable = 0;
  
  int devices=lc.getDeviceCount();
  for(int address=0;address<devices;address++) {
    /*The MAX72XX is in power-saving mode on startup*/
    lc.shutdown(address,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(address,6);
    /* and clear the display */
    lc.clearDisplay(address);
  }
}

void loop() {
    //writeDisplay();
    //run control structure if the ball is present
    if(isBallOnTable) {
      xAngle = constrain(xAngle - PID(xLoc - xTarget), 65, 115);
      yAngle = constrain(yAngle + PID(yLoc - yTarget), 65, 115);
      //xAngle = constrain(xAngle - (xLoc - xTarget), 65, 115);
      //yAngle = constrain(yAngle + (yLoc - yTarget), 65, 115);
      xServo.write(xAngle); // Only allowing 15 degrees of correction for now 
      yServo.write(yAngle); // Only allowing 15 degrees of correction for now
    }
    else {
      xServo.write(xResetAngle); // Set table to flat if ball is not present
      yServo.write(yResetAngle); // Set table to flat if ball is not present
      xAngle = xResetAngle;
      yAngle = yResetAngle;
    }
    delay(10);
}

int PID(float error) {
  float pWeight = error*pGain;
  iAccum = constrain(iAccum + error, -20, 20);
  float iWeight = iAccum*iGain;
  for(int i = 0; i<3; i++) {
    dBuffer[i] = dBuffer[i+1];
  }
  dBuffer[3] = error-dBuffer[2];
  int average = 0;
  for(int i = 0; i<4; i++) {
    average = average + dBuffer[i];
  }
  average = average/4;
  float dWeight = average*dGain;
  return (int)round(pWeight);//(int)round(pWeight + iWeight + dWeight);
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
      
    dataPiece = dataPiece+1;
    writeInt(xTarget);
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

void writeDisplay()
{
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  writeNumber(1,7,xLoc);
  writeNumber(1,3,yLoc);
  writeNumber(0,7,xTarget);
  writeNumber(0,3,yTarget);
}

void writeNumber(int board, int loc, int x)
{
  int ones;
  int tens;
  int hundreds;
  int thousands;
  ones = x%10;
  x = x/10;
  tens = x%10;
  x = x/10;
  hundreds = x%10;
  x = x/10;
  thousands = x;
  lc.setChar(board,loc,thousands,false);
  lc.setChar(board,loc-1,hundreds,false);
  lc.setChar(board,loc-2,tens,false);
  lc.setChar(board,loc-3,ones,false);
}
  
