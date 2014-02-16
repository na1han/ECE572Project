#include <Average.h>
#include <Servo.h> 

int xTarget;                     // Target location for the ball from the computer
int yTarget;                    
int xLoc;                        // Actual loction of the ball from the computer
int yLoc;
int xAngle;                      // Current table angle
int yAngle;
int xResetAngle;                 // Allow for reset angle to be something other than 90
int yResetAngle;  
int isBallOnTable;                // 1 if ball is present 0 if not
Servo xServo;  // create servo object to control the rotation around the x axis
Servo yServo;  // create servo object to control the rotation around the x axis
double pGain = 1;
double iGain = .01;
double dGain = 1;
double dBuffer[] = {0, 0, 0, 0};
double iAccum = 0;

void setup() {
  // initialize serial:
  Serial.begin(115200);
  xServo.attach(9);  // attaches the servo on pin 9 to the servo object
  yServo.attach(8);  // attaches the servo on pin 8 to the servo object
  xAngle = 90; 
  yAngle = 90;
  StartUp();
  xResetAngle = 90;
  yResetAngle = 90;
  xServo.write(xResetAngle); // Set table to flat if ball is not present
  yServo.write(yResetAngle); // Set table to flat if ball is not present
  isBallOnTable = 0;
}

void loop() {
  while (Serial.available()) {
    // transmit serial data "xTarget,yTarget,xLoc,yLoc,isBallOnTable"
    xTarget = Serial.parseInt();
    yTarget = Serial.parseInt();
    xLoc = Serial.parseInt();
    yLoc = Serial.parseInt();
    isBallOnTable = Serial.parseInt();
    
    if(isBallOnTable) {
      xAngle = xAngle + PID(xLoc - xTarget);
      yAngle = yAngle + PID(yLoc - yTarget);
      xServo.write(constrain(xAngle, 75, 105)); // Only allowing 15 degrees of correction for now 
      yServo.write(constrain(yAngle, 75, 105)); // Only allowing 15 degrees of correction for now
    }
    else {
      xServo.write(xResetAngle); // Set table to flat if ball is not present
      yServo.write(yResetAngle); // Set table to flat if ball is not present
      xAngle = 90;
      yAngle = 90;
    }
  }
}

int PID(double error) {
  double pWeight = error*pGain;
  iAccum = iAccum + error/10;
  double iWeight = iAccum*iGain;
  for(int i = 0; i<3; i++) {
    dBuffer[i] = dBuffer[i+1];
  }
  dBuffer[3] = error-dBuffer[2];
  int average = 0;
  for(int i = 0; i<4; i++) {
    average = average + dBuffer[i];
  }
  average = average/4;
  double dWeight = average*dGain;
  return (int)round(pWeight + iWeight + dWeight);
}

void StartUp() {
  int t = 0;
  for(t = 0; t < 40; t++) {
    xServo.write(xAngle++);
    yServo.write(yAngle++);
    delay(10);
  }
  for(t = 0; t < 80; t++) {
    xServo.write(xAngle--);
    yServo.write(yAngle--);
    delay(10);
  }
  for(t = 0; t < 40; t++) {
    xServo.write(xAngle++);
    yServo.write(yAngle++);
    delay(10);
  }
  xAngle = 90;
  yAngle = 90;
}


