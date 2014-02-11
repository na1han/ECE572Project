#include <Servo.h> 

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int xTarget;                     // Target location for the ball from the computer
int yTarget;                    
int xLoc;                        // Actual loction of the ball from the computer
int yLoc;
int xCorrection;                 // PID angle correction
int yCorrection;
int xAngle;                      // Current table angle
int yAngle;
int xResetAngle;                 // Allow for reset angle to be something other than 90
int yResetAngle;  
int isBallOnTable                // 1 if ball is present 0 if not
Servo xServo;  // create servo object to control the rotation around the x axis
Servo yServo;  // create servo object to control the rotation around the x axis

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  
  xServo.attach(9);  // attaches the servo on pin 9 to the servo object
  yServo.attach(8);  // attaches the servo on pin 8 to the servo object
  xResetAngle = 90;
  yResetAngle = 90;
  xServo.write(xResetAngle) // Set table to flat if ball is not present
  yServo.write(yResetAngle) // Set table to flat if ball is not present
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString); 
    // clear the string:
    inputString = "";
    stringComplete = false;
    if(isBallOnTable) {
      //TODO PID corrections
      
      xAngle = xAngle + xCorrection;
      yAngle = yAngle + yCorrection;
      xServo.write(constrain(xAngle, 75, 105)); // Only allowing 15 degrees of correction for now 
      yServo.write(constrain(yAngle, 75, 105)); // Only allowing 15 degrees of correction for now
    }
    else {
      xServo.write(xResetAngle) // Set table to flat if ball is not present
      yServo.write(yResetAngle) // Set table to flat if ball is not present
    }
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
  
    if (inChar == 'i') {              // 'i' transmission means the next byte will be the target x location
      xTarget = (int)Serial.read();
    }
    elseif (inChar == 'j') {          // 'j' transmission means the next byte will be the target y location
      yTarget = (int)Serial.read();
    }
    elseif (inChar == 'x') {          // 'x' transmission means the next byte will be the actual x location
      xLoc = (int)Serial.read();
    }
    elseif (inChar == 'y') {          // 'y' transmission means the next byte will be the target y location
      yLoc = (int)Serial.read();
    }
    elseif (inChar == 'b') {          // 'b' transmission means the next byte will be a 1 or 0 whether the ball is there or not
      isBallOnTable = (int)Serial.read();
    }
    elseif (inChar == '\n') {             // '\n' means that all the data has been loaded and control can begin
      stringComplete = true;
    } 
  }
}

