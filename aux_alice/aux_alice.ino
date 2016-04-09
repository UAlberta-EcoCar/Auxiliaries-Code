#include <Timer.h>
#include "aux_can.h"
#include <Servo.h>
Servo servo;

int initAngle = 10; 
int maxAngle = 140;
int angle;
int wiperDirection; // +1: Forward    -1: Reverse
int increment = 1;

int pauseInterval = 5; 
unsigned long previousTime = 0;
unsigned long currentTime;

Can myCan;
Timer indicatorTimer;

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_0, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(HORN_PIN, OUTPUT);
  pinMode(WIPER_PIN, OUTPUT);
  pinMode(BLINK_L_PIN, OUTPUT);
  pinMode(BLINK_R_PIN, OUTPUT);
  pinMode(F_DRIVING_LIGHTS_PIN, OUTPUT);
  pinMode(R_DRIVING_LIGHTS_PIN, OUTPUT);
  pinMode(BRK_L_PIN, OUTPUT);
  pinMode(BRK_R_PIN, OUTPUT);

  myCan.begin();
  indicatorTimer.reset();

  servo.attach(WIPER_PIN);
  angle = initAngle;
}

bool blinker = false;
bool indicator_left_flag = false;
bool indicator_right_flag = false;
bool hazards_flag = false;
bool brake_flag = false;

void loop() {
  myCan.read();
  currentTime = millis();

  // HORN
  if(myCan.horn_available()) {
    Serial.print("Horn ");
    Serial.println(myCan.horn() ? "On" : "Off");
    digitalWrite(HORN_PIN, myCan.horn() ? HIGH : LOW);
  }

  // HEADLIGHTS
  if(myCan.headlights_available()) {
    Serial.print("Headlights ");
    Serial.println(myCan.headlights() ? "On" : "Off");
    digitalWrite(F_DRIVING_LIGHTS_PIN, myCan.headlights() ? HIGH : LOW);
    digitalWrite(R_DRIVING_LIGHTS_PIN, myCan.headlights() ? HIGH : LOW);
  }

  // WIPERS
  if(myCan.wipers_available()) {
    Serial.print("Wipers ");
    Serial.println(myCan.wipers() ? "On" : "Off");
  }

  if (myCan.wipers_available()){
    if ((currentTime - previousTime) == pauseInterval){
      previousTime = currentTime;
      if (angle == initAngle){
        wiperDirection = 1; 
        angle = angle + increment;
      }
      else if ((angle > initAngle) && (angle < maxAngle) && (wiperDirection == 1)){
        angle = angle + increment;
      }
      else if (angle == maxAngle){
        wiperDirection = -1;
        angle = angle - increment;
      }    
      else {
        angle = angle - increment; 
      }
      servo.write(angle);
    }
  }
  else {
    if ((currentTime - previousTime) == pauseInterval){
      previousTime = currentTime;
      if ((angle > initAngle) && (angle <= maxAngle)){
        wiperDirection = -1;
        angle = angle - increment;
      }
      else {
        wiperDirection = 1;
      }
      servo.write(angle);
    }
  }


  // BRAKES
  if(myCan.brake_available()) {
    Serial.print("Brake ");
    Serial.println(myCan.brake() ? "On" : "Off");
    brake_flag = myCan.brake();
    digitalWrite(BRK_L_PIN, myCan.brake() ? HIGH : LOW);
    digitalWrite(BRK_R_PIN, myCan.brake() ? HIGH : LOW);
  }

  // SIGNALS
  if(myCan.signal_available()) {
    Serial.print("Signal ");
    switch(myCan.signal()){
      case LEFT_SIG: Serial.println("Left");
      indicator_left_flag = true; indicator_right_flag = false; blinker = true;
      break;
      case RIGHT_SIG: Serial.println("Right");
      indicator_right_flag = true; indicator_left_flag = false; blinker = true;
      break;
      case HARZARDS_SIG: Serial.println("Hazards");
      hazards_flag = true; indicator_left_flag = false; indicator_right_flag = false;
      blinker = true;
      break;
      default: Serial.println("Off");
      signal(false, false);
      indicator_left_flag = false;
      indicator_right_flag = false;
      hazards_flag = false;
      blinker = false;
    }
  }

  // Signal blinking
  if(indicatorTimer.elapsed(500)) {
    if(indicator_left_flag) {
      signal(blinker, false);
      blinker = !blinker;
    }
    if(indicator_right_flag) {
      signal(false, blinker);
      blinker = !blinker;
    }
    if(hazards_flag) {
      signal(blinker, blinker);
      blinker = !blinker;
    }
  }

}

// function to control blink lights
void signal(bool left, bool right) {
  digitalWrite(BLINK_L_PIN, left);
  digitalWrite(BLINK_R_PIN, right);
  // TODO: Handle brake with indicator
  digitalWrite(BRK_L_PIN, left);
  digitalWrite(BRK_R_PIN, right);
  digitalWrite(LED_0, left);
  digitalWrite(LED_1, right);
}
