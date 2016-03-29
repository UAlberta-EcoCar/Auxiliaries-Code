#include "aux_can.h"

Can myCan;

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
}

bool blinker = false;

void loop() {
  myCan.read();

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

  // BRAKES
  if(myCan.brake_available()) {
    Serial.print("Brake ");
    Serial.println(myCan.brake() ? "On" : "Off");
    digitalWrite(BRK_L_PIN, myCan.brake() ? HIGH : LOW);
    digitalWrite(BRK_R_PIN, myCan.brake() ? HIGH : LOW);
  }

  // SIGNALS
  if(myCan.signal_available()) {
    Serial.print("Signal ");
    switch(myCan.signal()){
      case LEFT_SIG: Serial.println("Left"); break;
      case RIGHT_SIG: Serial.println("Right"); break;
      case HARZARDS_SIG: Serial.println("Hazards"); break;
      default: Serial.println("Off");
    }
  }

}
