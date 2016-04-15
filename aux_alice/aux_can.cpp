#include "aux_can.h"

// LCD Horn
can_msg::MsgEncode horn_msg( can_msg::BOOL, can_msg::AUX, can_msg::HORN, can_msg::IMPORTANT, 1);
// LCD Wipers
can_msg::MsgEncode wipers_msg( can_msg::BOOL, can_msg::AUX, can_msg::WIPERS, can_msg::IMPORTANT, 1);
// LCD Signals
can_msg::MsgEncode signals_msg( can_msg::BOOL, can_msg::AUX, can_msg::SIGNAL, can_msg::IMPORTANT, 3);
// LCD Headlight
can_msg::MsgEncode headlights_msg( can_msg::BOOL, can_msg::AUX, can_msg::HEADLIGHTS, can_msg::IMPORTANT, 1);
// Brakes
can_msg::MsgEncode brake_msg( can_msg::UINT8, can_msg::MOTOR, can_msg::BRAKE, can_msg::CRITICAL, 1 );

/*
 * Initializes CAN bus
 */
void Can::begin() {
  // Initialize CAN
  Serial.println("Initializing CAN Controller");
  if (can_init(0,0,0,0,0,0,0,0)){
    Serial.println("Error: CAN initialization :(");
    while(1); // hang up program
  }
  Serial.println("CAN Controller Initialized :)");
}

/*
 * Read CAN Bus
 */
 void Can::read() {
   if(digitalRead(CAN_INT) == 0) {
     CanMessage message = can_get_message();
     if(message.id != 0)
     {
     Serial.println("MESSAGE RECIEVED");
     Serial.println(message.id);
     // read headlights
     if( message.id == headlights_msg.id()) {
       _headlights = message.data[0];
       _headlights_flag = true;
     }
     // read wipers
     if( message.id == wipers_msg.id() ) {
       _wipers = message.data[0];
       _wipers_flag = true;
     }
     // read horn
     if( message.id == horn_msg.id() ) {
       _horn = message.data[0];
       _horn_flag = true;
     }
     // read brake
     if( message.id == brake_msg.id() ) {
       _brake = message.data[0] > 0;
       _horn_flag = true;
     }
     // read signals
     if( message.id == signals_msg.id() ) {
       if(message.data[0] & 1 << can_msg::LEFT_SIGNAL) _signal = LEFT_SIG;
       else if (message.data[0] & 1 << can_msg::RIGHT_SIGNAL) _signal = RIGHT_SIG;
       else if (message.data[0] & 1 << can_msg::HAZARD_LIGHTS) _signal = HARZARDS_SIG;
       else _signal = 0;
       _signal_flag = true;
     }
     }
   }
 }
