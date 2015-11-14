#include "aux_can_handler.h"

#define HORN 0 //Horn
#define BLINK_L 1 //Left front lights
#define BLINK_R 2 //Right front lights
#define WIPER_SWITCH 3 //Wiper switch
#define HAZ_SWITCH 4 //Hazard lights switch
#define HORN_SWITCH 5 //Horn switch
#define L_SWITCH 6 //Left turn switch
#define R_SWITCH 7 //Right turn switch
#define R_LED 8 //Right dashboard blinker
#define L_LED 9 //Left dashboard blinker
#define BRK_L_HIGH 10 //Left back lights
#define WIPER_OUTPUT 11 //Wipers output (servo motor)
#define BRK_R_HIGH 12 //Right back lights
#define BRK_INPUT 13 //Break input
//Driving lights are not defined for the testing purposes

int BlinkTimeInterval = 500;
boolean BlinkOn = false;
boolean WiperDirection = false; //true is up, false is down
boolean HazLightsOn = false;
boolean RBlinkersOn = false;
boolean LBlinkersOn = false;
int WiperPosition = 0;
long CurrentTime = 0;
long BlinkComparisonTime = 0;
long WiperTime = 0;
int WiperIncrementTime = 2;

void setup(){

  //start CAN bus
  aux_can_init();
  
  //pinMode(DRIVING_LIGHTS, OUTPUT); not defined in testing purposes
  pinMode(HORN, OUTPUT);
  pinMode(BLINK_L, OUTPUT);
  pinMode(BLINK_R, OUTPUT);
  pinMode(WIPER_SWITCH, INPUT);
  pinMode(HAZ_SWITCH, INPUT);
  pinMode(HORN_SWITCH, INPUT);
  pinMode(L_SWITCH, INPUT);
  pinMode(R_SWITCH, INPUT);
  pinMode(R_LED, OUTPUT);
  pinMode(L_LED, OUTPUT);
  pinMode(BRK_L_HIGH, OUTPUT);
  pinMode(WIPER_OUTPUT, OUTPUT);
  pinMode(BRK_R_HIGH, OUTPUT);
  pinMode(BRK_INPUT, INPUT);
  //digitalWrite(DRIVING_LIGHTS, HIGH); Taken out for testing purposes
}

void Break_Lights(boolean HazLightsOn, boolean LLightsOn, boolean RLightsOn){
  if (!HazLightsOn){
    if (LLightsOn){
      digitalWrite(BRK_R_HIGH, (digitalRead(BRK_INPUT)));  
    }  
    else if (RLightsOn){
      digitalWrite(BRK_L_HIGH, (digitalRead(BRK_INPUT)));  
    }
    else{
      digitalWrite(BRK_L_HIGH, (digitalRead(BRK_INPUT))); 
      digitalWrite(BRK_R_HIGH, (digitalRead(BRK_INPUT)));
    }
  }
}

int Set_Wiper_Position(int WiperPosition, boolean Direction){
  if ((WiperPosition == 0) && (digitalRead(WIPER_SWITCH) == LOW)){
    WiperPosition = 0;
  }
  else if (Direction){
    WiperPosition++;
  }
  else{
    WiperPosition--;  
  }
  analogWrite(WIPER_OUTPUT, WiperPosition);
  return WiperPosition;
}

boolean Wiper_Direction(int WiperPosition, boolean Direction){
  if (digitalRead(WIPER_SWITCH) == HIGH){
    if (Direction){
      if (WiperPosition < 255){
        Direction = true;  
      }
      else{
        Direction = false;  
      }
    }
    else{
      if (WiperPosition > 0){
        Direction = false;  
      }
      else{
        Direction = true;  
      }
    }
  }
  else{
    Direction = false;
  }
  return Direction;
}

void Horn(){
  digitalWrite(HORN, (digitalRead(HORN_SWITCH)));
}

boolean Left_Turn_Light(boolean BlinkingOn){
  boolean LightsOn;
  if (digitalRead(L_SWITCH) == HIGH){
    LightsOn = true;
    if (BlinkingOn){
      digitalWrite(L_LED, HIGH);
      digitalWrite(BLINK_L, HIGH);
      digitalWrite(BRK_L_HIGH, HIGH);  
    }
    else{
      digitalWrite(L_LED, LOW);
      digitalWrite(BLINK_L, LOW);
      digitalWrite(BRK_L_HIGH, LOW);  
    }
  }
  else{
    LightsOn = false;
    digitalWrite(L_LED, LOW);
    digitalWrite(BLINK_L, LOW);
    digitalWrite(BRK_L_HIGH, LOW);  
  }
  return LightsOn;
}

boolean Right_Turn_Light(boolean BlinkingOn){
  boolean LightsOn;
  if (digitalRead(R_SWITCH) == HIGH){
    LightsOn = true;
    if (BlinkingOn){
      digitalWrite(R_LED, HIGH);
      digitalWrite(BLINK_R, HIGH);
      digitalWrite(BRK_R_HIGH, HIGH);  
    }
    else{
      digitalWrite(R_LED, LOW);
      digitalWrite(BLINK_R, LOW);
      digitalWrite(BRK_R_HIGH, LOW);  
    }
  }
  else{
    LightsOn = false;
    digitalWrite(R_LED, LOW);
    digitalWrite(BLINK_R, LOW);
    digitalWrite(BRK_R_HIGH, LOW);  
  }
  return LightsOn;
}

boolean Hazard_Lights(boolean BlinkingOn){
  boolean HazLightsOn;
  if (digitalRead(HAZ_SWITCH) == HIGH){
    HazLightsOn = true;
    if (BlinkingOn){
      digitalWrite(R_LED, HIGH);
      digitalWrite(BLINK_R, HIGH);
      digitalWrite(BRK_R_HIGH, HIGH);
      digitalWrite(L_LED, HIGH);
      digitalWrite(BLINK_L, HIGH);
      digitalWrite(BRK_L_HIGH, HIGH);
    }
    else{
      digitalWrite(R_LED, LOW);
      digitalWrite(BLINK_R, LOW);
      digitalWrite(BRK_R_HIGH, LOW);
      digitalWrite(L_LED, LOW);
      digitalWrite(BLINK_L, LOW);
      digitalWrite(BRK_L_HIGH, LOW);
    }
  }
  else{
    HazLightsOn = false;  
    digitalWrite(R_LED, LOW);
    digitalWrite(BLINK_R, LOW);
    digitalWrite(BRK_R_HIGH, LOW);
    digitalWrite(L_LED, LOW);
    digitalWrite(BLINK_L, LOW);
    digitalWrite(BRK_L_HIGH, LOW);
  }
  return HazLightsOn;
}

void loop(){
  if(digitalRead(8)) //or whatever pin is connected to CAN_INT
  {
    aux_read_can_bus();
  }
  
  CurrentTime = millis();
  if ((CurrentTime - BlinkComparisonTime) > BlinkTimeInterval){
    BlinkOn = true;
    if ((CurrentTime - BlinkComparisonTime) > 2*(BlinkTimeInterval)){
      BlinkComparisonTime = CurrentTime;
    }
  }
  else{
    BlinkOn = false;
  }
  Horn();
  HazLightsOn = Hazard_Lights(BlinkOn);
  LBlinkersOn = Left_Turn_Light(BlinkOn);
  RBlinkersOn = Right_Turn_Light(BlinkOn);
  Break_Lights(HazLightsOn, LBlinkersOn, RBlinkersOn);
  if (CurrentTime > (WiperTime + WiperIncrementTime)){
    WiperDirection = Wiper_Direction(WiperPosition, WiperDirection);
    WiperPosition = Set_Wiper_Position(WiperPosition, WiperDirection);
    WiperTime = CurrentTime;
  }
}
