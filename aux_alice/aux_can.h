#ifndef AUX_CAN_H
#define AUX_CAN_H

#include <Arduino.h>
#include <mcp2515_lib.h>
#include <mcp2515_filters.h>
#include <can_message.h>
#include <can_message_def.h>
#include "hardware.h"

#define LEFT_SIG 1
#define RIGHT_SIG 2
#define HARZARDS_SIG 3

class Can {
public:
  Can() {};
  void begin();

  void read();
  bool horn_available() { return _horn_flag; }
  bool horn() { _horn_flag = false; return _horn; }
  bool headlights_available() { return _headlights_flag; }
  bool headlights() { _headlights_flag = false; return _headlights; }
  bool wipers_available() { return _wipers_flag; }
  bool wipers() { _wipers_flag = false; return _wipers; }
  bool brake_available() { return _brake_flag; }
  bool brake() { _brake_flag = false; return _brake; }
  bool signal_available() { return _signal_flag; }
  uint8_t signal() { _signal_flag = false; return _signal; }
private:
  bool _horn, _horn_flag;
  bool _headlights, _headlights_flag;
  bool _wipers, _wipers_flag;
  bool _brake, _brake_flag;
  uint8_t _signal; bool _signal_flag;
};

#endif
