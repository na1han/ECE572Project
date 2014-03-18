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
float pGain = 1;
float iGain = .01;
float dGain = 1;
float dBuffer[] = {0, 0, 0, 0};
float iAccum = 0;
char temp;


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
  //StartUp();
  //xServo.write(xResetAngle); // Set table to flat if ball is not present
  //yServo.write(yResetAngle); // Set table to flat if ball is not present
  isBallOnTable = 0;
}

void loop() {
    // transmit serial data "xTarget yTarget xLoc yLoc isBallOnTable"
    xTarget = ReadInt16();
    yTarget = ReadInt16();
    xLoc = ReadInt16();
    yLoc = ReadInt16();
    isBallOnTable = ReadInt16();
    //Echo recieved data
    WriteInt16(xTarget);
    WriteInt16(yTarget);
    WriteInt16(xLoc);
    WriteInt16(yLoc);
    WriteInt16(isBallOnTable);


    //run control structure if the ball is present
    if(isBallOnTable) {
      xAngle = xAngle + PID(xLoc - xTarget);
      yAngle = yAngle + PID(yLoc - yTarget);
      //xServo.write(constrain(xAngle, 55, 125)); // Only allowing 15 degrees of correction for now 
      //yServo.write(constrain(yAngle, 55, 1125)); // Only allowing 15 degrees of correction for now
    }
    else {
      //xServo.write(xResetAngle); // Set table to flat if ball is not present
      //yServo.write(yResetAngle); // Set table to flat if ball is not present
      xAngle = xResetAngle;
      yAngle = yResetAngle;
    }
}

int PID(float error) {
  float pWeight = error*pGain;
  iAccum = constrain(iAccum + error/10, -20, 20);
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
  return (int)round(pWeight + iWeight + dWeight);
}

void StartUp() {
  int t = 0;
  int pause = 5;
  Serial.println("Start up sequence");
  delay(2000);
  for(t = 0; t < 35; t++) {
    xServo.write(xAngle++);
    yServo.write(yAngle++);
    delay(pause);
  }
  for(t = 0; t < 70; t++) {
    xServo.write(xAngle--);
    delay(pause);
  }
  for(t = 0; t < 70; t++) {
    yServo.write(yAngle--);
    delay(pause);
  }
  for(t = 0; t < 70; t++) {
    xServo.write(xAngle++);
    delay(pause);
  }
  for(t = 0; t < 70; t++) {
    yServo.write(yAngle++);
    delay(pause);
  }
  for(t = 0; t < 35; t++) {
    xServo.write(xAngle--);
    yServo.write(yAngle--);
    delay(pause);
  }
  Serial.println("Ready to run");
}

int ReadInt16() {
  int in;
  while(Serial.available() < 2);
  in = Serial.read() << 8;
  in += Serial.read() & 0x00FF;
  return in;
}

void WriteInt16(int data) {
  Serial.flush();
  Serial.write((byte)(data >> 8));
  Serial.flush();
  Serial.write((byte)(data & 0x00FF));
}

