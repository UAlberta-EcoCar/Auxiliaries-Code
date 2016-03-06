

#ifndef AUX_CAN_HANDLER_H
#define AUX_CAN_HANDLER_H

#include <arduino.h> //needed for the uint8_t type definition

uint8_t aux_can_init(void);
void aux_read_can_bus(void);
uint8_t check_brake(void);
uint8_t check_left_signal(void);
uint8_t check_right_signal(void);
uint8_t check_horn(void);
uint8_t check_wipers(void);

#endif

