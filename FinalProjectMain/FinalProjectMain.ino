String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int xTarget;
int yTarget;
int xLoc;
int yLoc;


void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString); 
    // clear the string:
    inputString = "";
    stringComplete = false;
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
    if (inChar == '\n') {             // '\n' means that all the data has been loaded and control can begin
      stringComplete = true;
    } 
  }
}

