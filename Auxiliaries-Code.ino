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
#define BRK_R_HIGH 2 //Right back lights

int ReadTimeInterval = 250;
int BlinkTimeInterval = 500;
boolean BlinkOn = false;
long CurrentTime = 0;
long ComparisonTime = 0;

void setup(){
  pinMode(DRIVING_LIGHTS, OUTPUT);
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
  pinMode(BRK_R_HIGH, OUTPUT);
  digitalWrite(DRIVING_LIGHTS, HIGH);
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

boolean Hazard_Lights(boolean BlinkingOn){
  boolean HazLightsOn;
  if ((digitalRead(HAZ_SWITCH) == HIGH) && (BlinkingOn)){
    HazLightsOn = true;
    if (BlinkingOn){
     digitalWrite(R_LED, HIGH);
     digitalWrite(BLINK_R, HIGH);
     digitalWrite(BRK_R_HIGH, HIGH);
     digitalWrite(L_LED, HIGH);
     digitalWrite(BLINK_L, HIGH);
     digitalWrite(BRK_L_HIGH, HIGH);
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
  CurrentTime = millis();
  if ((CurrentTime - ComparisonTime) < BlinkTimeInterval){
    BlinkOn = true;
    ComparisonTime = CurrentTime;
  }
  else{
    BlinkOn = false;
  }
}
