#include <Timer.h>
#include "aux_can.h"
#include <Servo.h>

Can myCan;

Timer indicatorTimer;
Timer status_send_timer;
Timer horn_timer;

Servo myServo;
#define restPosition 0
#define farPosition 180
int16_t servoPosition = restPosition;
int8_t servoDirection = 1;
#define servoSweep_speed 5
unsigned long servoSweep_timer;

uint32_t flash_timer;

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

  pinMode(A0,INPUT);

  myCan.begin();
  
  indicatorTimer.reset();
  
  myServo.attach(5);
  myServo.write(servoPosition);
}

bool blinker = false;
bool indicator_left_flag = false;
bool indicator_right_flag = false;
bool hazards_flag = false;
bool brake_flag = false;

bool reegan_brake_flag = false;
bool reegan_brake = false;

uint32_t brk_timer;

bool flash;

void loop() {
  myCan.read();
  if(millis() - flash_timer > 500)
  {
    flash ^= 1;
    flash_timer=millis();
  }

  // HORN
  if(myCan.horn_available()) {
    horn_timer.reset();
    Serial.print("Horn ");
    Serial.println(myCan.horn() ? "On" : "Off");
    digitalWrite(HORN_PIN, myCan.horn() ? HIGH : LOW);
  }
  if(horn_timer.elapsed(750))
  {
    digitalWrite(HORN_PIN,LOW);
  }


  // WIPERS
  myServo.write(servoPosition);
  if(myCan.wipers_available()) {
    Serial.print("Wipers ");
    Serial.println(myCan.wipers() ? "On" : "Off");
  }
  if(myCan.wipers())
  {
    if(millis() - servoSweep_timer > servoSweep_speed)
    {
      servoSweep_timer = millis();
      servoPosition = servoPosition + servoDirection;
//      Serial.println(servoPosition);
    }
    if(servoPosition == farPosition)
    {
      servoDirection = -1;
    }
    if(servoPosition == restPosition)
    {
      servoDirection = 1;
    }
  }
  else
  {    
    if(millis() - servoSweep_timer > servoSweep_speed)
    {
      servoSweep_timer = millis();
      servoPosition = servoPosition + servoDirection;
//      Serial.println(servoPosition);
    }
    if(servoPosition == farPosition)
    {
      servoDirection = -1;
    }
    if(servoPosition == restPosition)
    {
      servoDirection = 0;
    }
    else
    {
      servoDirection = -1;
    }
  }

  digitalWrite(F_DRIVING_LIGHTS_PIN,myCan.headlights());
  digitalWrite(R_DRIVING_LIGHTS_PIN,myCan.headlights());

  if(analogRead(A0) < 800)
  {
    brk_timer = millis();
    Serial.println(analogRead(A0));
  }

  if (millis()-brk_timer<250)
  {
    reegan_brake = 1;
  }
  else
  {
    reegan_brake = 0;
  }

  digitalWrite(BRK_R_PIN,reegan_brake);
  digitalWrite(BRK_L_PIN,reegan_brake);

  digitalWrite(BLINK_L_PIN,LOW);
  digitalWrite(BLINK_R_PIN,LOW);
  
  if(myCan.signal() == LEFT_SIG)
  {
    digitalWrite(BRK_L_PIN,flash);
    digitalWrite(BLINK_L_PIN,flash);
  }

  if(myCan.signal() == RIGHT_SIG)
  {
    digitalWrite(BRK_R_PIN,flash);
    digitalWrite(BLINK_R_PIN,flash);
  }

  if(myCan.signal() == HAZARDS_SIG)
  {
    digitalWrite(BRK_R_PIN,flash);
    digitalWrite(BRK_L_PIN,flash);
    digitalWrite(BLINK_L_PIN,flash);
    digitalWrite(BLINK_R_PIN,flash);
  }

  digitalWrite(LED_0,digitalRead(BRK_R_PIN));
  digitalWrite(LED_1,digitalRead(BRK_L_PIN));
}


