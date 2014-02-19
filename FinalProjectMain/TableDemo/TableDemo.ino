#include <Servo.h> 

//This demos the table tilting back and forth, you can use the Arduino
// Serial window to send it new tilt angles and pauses in ms
// 

int tilt;                     
int pause;
Servo xServo;
Servo yServo;
char temp;

void setup() {
  // initialize serial:
  Serial.begin(115200);
  xServo.attach(9);  // attaches the servo on pin 9 to the servo object
  yServo.attach(8);  // attaches the servo on pin 8 to the servo object
  tilt = 0;
  pause = 166;
}

void loop() {
  delay(pause);
  xServo.write(90-tilt);
  yServo.write(90-tilt);
  delay(pause);
  xServo.write(90+tilt);  
  yServo.write(90+tilt);
  while (Serial.available()) {
    // transmit serial data "tilt,pause(ms),"
    tilt = constrain(Serial.parseInt(), 0, 35);
    pause = constrain(Serial.parseInt(), 150, 5000);
    temp = Serial.read();
    //Echo recieved data
    Serial.print(tilt);
    Serial.print(",");
    Serial.println(pause);
  }
}


