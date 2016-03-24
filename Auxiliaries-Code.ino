#include "aux_can_handler.h"
#include "Aux_defs.h"

int BlinkTimeInterval = 500;
boolean Blink;
long BlinkComparisonTime = 0;

bool brake_state;
bool hazard_state;
bool left_signal_state;
bool right_signal_state;
  
void setup(){
  Serial.begin(115200);

  //start CAN bus
  aux_can_init();
  digitalWrite(LED_1,HIGH);
  
  //pinMode(DRIVING_LIGHTS, OUTPUT); not defined in testing purposes
  pinMode(HORN, OUTPUT);
  pinMode(BLINK_L, OUTPUT);
  pinMode(BLINK_R, OUTPUT);
  pinMode(LED_0, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(BRK_L_HIGH, OUTPUT);
  pinMode(WIPER_OUTPUT, OUTPUT);
  pinMode(BRK_R_HIGH, OUTPUT);
  pinMode(R_DRIVING_LIGHTS,OUTPUT);
  pinMode(F_DRIVING_LIGHTS,OUTPUT);

}


void loop()
{
  if(digitalRead(9) == 0)
  {
    digitalWrite(LED_1,LOW);
    aux_read_can_bus();
    digitalWrite(LED_1,HIGH);
  }

  if(millis() - BlinkComparisonTime > BlinkTimeInterval )
  {
    Blink ^= 1;
    digitalWrite(LED_0,Blink);
    BlinkComparisonTime = millis();
  }

  if(check_headlights())
  {
    digitalWrite(F_DRIVING_LIGHTS,HIGH);
    digitalWrite(R_DRIVING_LIGHTS,HIGH);
  }
  else
  {
    digitalWrite(F_DRIVING_LIGHTS,LOW);
    digitalWrite(R_DRIVING_LIGHTS,LOW);
  }
  
  if(check_horn())
  {
    digitalWrite(HORN,HIGH);
  }
  else
  {
    digitalWrite(HORN,LOW);
  }


  if(check_hazards())
  {
    digitalWrite(BRK_R_HIGH,Blink);
    digitalWrite(BRK_L_HIGH,Blink);
    digitalWrite(BLINK_R,Blink);
    digitalWrite(BLINK_L,Blink);
    hazard_state = 1;
  }
  else
  {
    if(hazard_state)
    {
      digitalWrite(BRK_R_HIGH,LOW);
      digitalWrite(BRK_L_HIGH,LOW);
      digitalWrite(BLINK_R,LOW);
      digitalWrite(BLINK_L,LOW);
      hazard_state = 0;
    }
  }

  //left signal
  if(check_left_signal())
  {
    digitalWrite(BRK_L_HIGH,Blink);
    digitalWrite(BLINK_L,Blink);
    left_signal_state = 1;

    if(Blink)
    {
      Serial.println("RIGHT");
    }
  }
  else
  {
   if(left_signal_state)
   {
    digitalWrite(BRK_L_HIGH,brake_state);
    digitalWrite(BLINK_L,LOW);
    left_signal_state = 0; 
   }
  }

  //right signal
  if(check_right_signal())
  {
    digitalWrite(BRK_R_HIGH,Blink);
    digitalWrite(BLINK_R,Blink);
    right_signal_state = 1;  }
  else
  {
    if(right_signal_state)
    {
      digitalWrite(BRK_R_HIGH,brake_state);
      digitalWrite(BLINK_R,LOW);
      right_signal_state = 0;
    }
  }

  //brake
  if(check_brake())
  {
    if(brake_state == 0)
    {
      digitalWrite(BRK_R_HIGH,HIGH);
      digitalWrite(BRK_L_HIGH,HIGH);
      brake_state = 1;
    }
  }
  else
  {
    if(brake_state)
    {
      digitalWrite(BRK_R_HIGH,LOW);
      digitalWrite(BRK_L_HIGH,LOW);
      brake_state = 0;
    }
  }
}
