/*
#define DRIVING_LIGHTS A5 //Driving lights
#define HORN A4 //Horn
#define BLINK_L A3 //Left front lights
#define BLINK_R A2 //Right front lights
#define WIPER_SWITCH A1 //Wiper switch
#define HAZ_SWITCH A0 //Hazard lights switch
#define HORN_SWITCH 9 //Horn switch
#define L_SWITCH 8 //Left turn switch
#define R_SWITCH 7 //Right turn switch
#define R_LED 6 //Right dashboard blinker
#define L_LED 5 //Left dashboard blinker
#define BRK_L_HIGH 4 //Left back lights
#define WIPER_OUTPUT 3 //Wipers output (servo motor)
#define BRK_R_HIGH 2 //Right back lights
//#define BRK_INPUT SOMETHING //Break input
//Check the position of these inputs and outputs on the board
*/

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

int ReadTimeInterval = 250;
int BlinkTimeInterval = 500;
boolean BlinkOn = false;
boolean HazLight = false;
boolean WiperDirection = false; //true is up, false is down
int WiperPosition = 0;
long CurrentTime = 0;
long BlinkComparisonTime = 0;
long WiperTime = 0;

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
  //pinMode(BRK_INPUT, INPUT);
  //digitalWrite(DRIVING_LIGHTS, HIGH); Taken out for testing purposes
}

boolean Break_Lights(){ //Remember to add BreakOn parameters for hazard lights and blinkers - Takes precedence over everything else
  boolean BreakOn;
  if (digitalRead(BRK_INPUT) == HIGH){
    BreakOn = true;
    digitalWrite(BRK_L_HIGH, HIGH);
    digitalWrite(BRK_R_HIGH, HIGH);
  }
  else{
    BreakOn = false;
    digitalWrite(BRK_L_HIGH, LOW);
    digitalWrite(BRK_R_HIGH, LOW);
  }
}

int Set_Wiper_Position(int WiperPosition, boolean Direction){
  if ((WiperPosition == 0) && (digitalRead(WIPER_SWITCH) == LOW)){
    WiperPosition = 0;
  }
  else if (Direction){
    WiperPosition++;
  }
  else if (!Direction){
    WiperPosition--;  
  }
  return WiperPosition;
  analogWrite(WIPER_OUTPUT, WiperPosition);
}

boolean Wiper_Direction(int WiperPosition, boolean Direction){
  if (digitalRead(WIPER_SWITCH) == HIGH){
    if (Direction){
      if (WiperPosition < 256){
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
  if (digitalRead(HORN_SWITCH) == HIGH){
    digitalWrite(HORN, HIGH);
  }
  else{
    digitalWrite(HORN, LOW);  
  }
}

void Right_Turn_Light(boolean BlinkingOn, boolean HazLightsOn){
  if (!HazLightsOn){
    if ((digitalRead(R_SWITCH) == HIGH) && (BlinkingOn)){
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
}

void Left_Turn_Light(boolean BlinkingOn, boolean HazLightsOn){
  if (!HazLightsOn){
    if ((digitalRead(L_SWITCH) == HIGH) && (BlinkingOn)){
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
    digitalWrite(R_LED, LOW);
    digitalWrite(BLINK_R, LOW);
    digitalWrite(BRK_R_HIGH, LOW);
    digitalWrite(L_LED, LOW);
    digitalWrite(BLINK_L, LOW);
    digitalWrite(BRK_L_HIGH, LOW);
    HazLightsOn = false;
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
  HazLight = Hazard_Lights(BlinkOn);
  Right_Turn_Light(BlinkOn, HazLight);
  Left_Turn_Light(BlinkOn, HazLight);
  if (CurrentTime > (WiperTime + 2)){
    WiperDirection = Wiper_Direction(WiperPosition, WiperDirection);
    WiperPosition = Set_Wiper_Position(WiperPosition, WiperDirection);
    WiperTime = CurrentTime;
  }
}
