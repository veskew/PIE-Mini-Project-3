#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

unsigned long previousMillis = 0; // Store the last time sensor values were checked
const long interval = 25;          // Interval at which to check sensor values (milliseconds)



int sensorPinLeft = A2;   // select the input pin for the left sensor
int sensorPinRight = A3;   // select the input pin for the right sensor

int sensorValueLeft = 0;  // variable to store the value coming from the left sensor
int sensorValueRight = 0; // variable coming from the right sensor

int timeSeenMS = 0;
int motorScale = 5;
int motorSpeedLeft = 0;
int motorSpeedRight = 0;
char receivedChar;
boolean newData = false;

int lastSeen = 2; // Left represents 1 right represents 0
// Create the motor shield object
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Connect to the motors
Adafruit_DCMotor *R_motor = AFMS.getMotor(1); // Motor connected to port M1
Adafruit_DCMotor *L_motor = AFMS.getMotor(2); // Motor connected to port M2

void setup() {
  Serial.begin(9600);           // sets up Serial library at 9600 bps
  Serial.println("DC Motor Test");
  Serial.println("Loop running...");
  
  // Initialize the motor shield
  AFMS.begin();

  // Set initial speed
  motorSpeedLeft = 5 * motorScale;
  motorSpeedRight = 5 * motorScale;

  L_motor->setSpeed(motorSpeedLeft);        // sets L motor speed
  R_motor->setSpeed(motorSpeedRight);        // sets R motor speed
 
  
  // Start motors
  R_motor->run(FORWARD);        // runs R motor forward
  L_motor->run(FORWARD);        // runs L motor forward
  delay(interval);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Save the last time the sensor was checked
    sensorValueLeft = analogRead(sensorPinLeft);
    sensorValueRight = analogRead(sensorPinRight);
    timeSeenMS += interval;
    csvFormat();

    if(sensorValueRight > 962) { //770
      Serial.println("Right Sensor sees tape");
      // check if last seen tape was on left sensor
      if(lastSeen == 1){
      motorSpeedLeft = 10 * motorScale;
      motorSpeedRight = 4 * motorScale;

      L_motor->setSpeed(motorSpeedLeft);        // sets L motor speed
      R_motor->setSpeed(motorSpeedRight);        // sets R motor speed
      delay(3);
      motorSpeedLeft = 7 * motorScale;
      motorSpeedRight = 1 * motorScale;

      L_motor->setSpeed(motorSpeedLeft);        // sets L motor speed
      R_motor->setSpeed(motorSpeedRight);        // sets R motor speed
      }
      else{
      motorSpeedLeft = 7 * motorScale;
      motorSpeedRight = 4 * motorScale;

      L_motor->setSpeed(motorSpeedLeft);        // sets L motor speed
      R_motor->setSpeed(motorSpeedRight);        // sets R motor speed
      delay(3);
      motorSpeedLeft = 5 * motorScale;
      motorSpeedRight = 1 * motorScale;

      L_motor->setSpeed(motorSpeedLeft);        // sets L motor speed
      R_motor->setSpeed(motorSpeedRight);        // sets R motor speed
      }
      lastSeen = 1;
      timeSeenMS = 0;

    }
    else if(sensorValueLeft > 862) { //945
      Serial.println("Left Sensor sees tape");
    // check if last seen tape was on left sensor
      if(lastSeen == 0){
      motorSpeedLeft = 4 * motorScale;
      motorSpeedRight = 10 * motorScale;

      L_motor->setSpeed(motorSpeedLeft);        // sets L motor speed
      R_motor->setSpeed(motorSpeedRight);        // sets R motor speed
      delay(3);
      motorSpeedLeft = 1 * motorScale;
      motorSpeedRight = 7 * motorScale;

      L_motor->setSpeed(motorSpeedLeft);        // sets L motor speed
      R_motor->setSpeed(motorSpeedRight);        // sets R motor speed
      }
      else{
      motorSpeedLeft = 4 * motorScale;
      motorSpeedRight = 7 * motorScale;

      L_motor->setSpeed(motorSpeedLeft);        // sets L motor speed
      R_motor->setSpeed(motorSpeedRight);        // sets R motor speed
      delay(3);
      motorSpeedLeft = 1 * motorScale;
      motorSpeedRight = 5 * motorScale;

      L_motor->setSpeed(motorSpeedLeft);        // sets L motor speed
      R_motor->setSpeed(motorSpeedRight);        // sets R motor speed
      }
      lastSeen = 0;
      timeSeenMS = 0;
    }
    else if(timeSeenMS > 1000){
      if(lastSeen == 0){
      motorSpeedLeft = 4 * motorScale;
      motorSpeedRight = 10 * motorScale;

      L_motor->setSpeed(motorSpeedLeft);        // sets L motor speed
      R_motor->setSpeed(motorSpeedRight);        // sets R motor speed
      delay(3);
      motorSpeedLeft = 1 * motorScale;
      motorSpeedRight = 7 * motorScale;

      L_motor->setSpeed(motorSpeedLeft);        // sets L motor speed
      R_motor->setSpeed(motorSpeedRight);        // sets R motor speed
      }
      else{
      motorSpeedLeft = 10 * motorScale;
      motorSpeedRight = 4 * motorScale;

      L_motor->setSpeed(motorSpeedLeft);        // sets L motor speed
      R_motor->setSpeed(motorSpeedRight);        // sets R motor speed
      delay(3);
      motorSpeedLeft = 7 * motorScale;
      motorSpeedRight = 1 * motorScale;

      L_motor->setSpeed(motorSpeedLeft);        // sets L motor speed
      R_motor->setSpeed(motorSpeedRight);        // sets R motor speed
      }
    }
  }
  recvOneChar();
  showNewData();

}
// code to check if a character was typed
void recvOneChar() {
    if (Serial.available() > 0) {
        receivedChar = Serial.read();
        newData = true;
    }
}
// code to perform an action based on inputted character
void showNewData() {
    if (newData == true) {
        Serial.print("This just in ... ");
        Serial.println(receivedChar);
        if(isDigit(receivedChar)){
          Serial.println(receivedChar);
          motorScale = receivedChar - '0';
        }
        newData = false;
    }
}

// code to print serial outputs that can be sent to python file easily
void csvFormat(){
  Serial.print(millis());
  Serial.print(",");
  Serial.print(sensorValueLeft);
  Serial.print(",");
  Serial.print(sensorValueRight);
  Serial.print(",");
  Serial.print(motorSpeedLeft);
  Serial.print(",");
  Serial.println(motorSpeedRight);
}

// code to print values to serial to help with mechanical debugging
void debugFormat(){
  Serial.print("Left sensor: ");
  Serial.println(sensorValueLeft);
  Serial.print("Right sensor: ");
  Serial.println(sensorValueRight);
  Serial.print("motorScale: ");
  Serial.println(motorScale);
  Serial.print("Left Motor speed: ");
  Serial.println(motorSpeedLeft);
  Serial.print("Right Motor speed: ");
  Serial.println(motorSpeedRight);
  
}