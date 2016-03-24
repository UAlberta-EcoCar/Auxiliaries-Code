#include "Blink_Patterns.h"
#include "Aux_defs.h"
#include <Arduino.h>

uint32_t timer;
uint32_t timer2;
uint8_t blink_switch;

void pattern_1(void)
{
  if(millis() - timer > 500)
  {
    blink_switch++; 
  }
  switch(blink_switch)
  {
    case 0:
      digitalWrite(F_DRIVING_LIGHTS,LOW);
      digitalWrite(R_DRIVING_LIGHTS,LOW);
  }
}

