#include "aux_can_handler.h"
#include <stdio.h>
#include <mcp2515_lib.h>
#include <mcp2515_filters.h>
#include <can_message.h>
#include <can_message_def.h>
#include <arduino.h>

//LCD Horn
can_msg::MsgEncode lcd_horn_msg( can_msg::BOOL, can_msg::AUX, can_msg::HORN, can_msg::IMPORTANT, 1);

//LCD WIpers
can_msg::MsgEncode lcd_wipers_msg( can_msg::BOOL, can_msg::AUX, can_msg::WIPERS, can_msg::IMPORTANT, 1);

//LCD Signals
can_msg::MsgEncode lcd_signals_msg( can_msg::BOOL, can_msg::AUX, can_msg::SIGNAL, can_msg::IMPORTANT, 3);

//LCD Headlight
can_msg::MsgEncode lcd_headlights_msg( can_msg::BOOL, can_msg::AUX, can_msg::HEADLIGHTS, can_msg::IMPORTANT, 1);

//BRAKES
can_msg::MsgEncode brake_msg( can_msg::UINT8, can_msg::MOTOR, can_msg::BRAKE, can_msg::CRITICAL, 1 );

uint8_t aux_can_init(void)
{
    //initialize CAN bus with filtering for aux and motor messages
    while(can_init(DEVICE_MASK|MESSAGE_MASK,lcd_horn_msg.id(),lcd_wipers_msg.id(),DEVICE_MASK|MESSAGE_MASK,lcd_signals_msg.id(),lcd_headlights_msg.id(),0,0));
    return (0);
}

#define BRAKE_BIT 0
#define LEFT_SIGNAL_BIT 1
#define RIGHT_SIGNAL_BIT 2
#define HAZARDS_BIT 3
#define HORN_BIT 4
#define WIPERS_BIT 5
#define HEADLIGHTS_BIT 6
uint8_t aux_input_state;


void aux_read_can_bus(void)
{
    CanMessage message;
    message = can_get_message();
    if ((message.id & (DEVICE_MASK|MESSAGE_MASK)) == (lcd_signals_msg.id() & (DEVICE_MASK|MESSAGE_MASK)))
    {
            if(message.data[0] & (1 << can_msg::LEFT_SIGNAL))
            {
                aux_input_state |= (1 << LEFT_SIGNAL_BIT);
            }
            else
            {
                aux_input_state &= ~(1 << LEFT_SIGNAL_BIT);
            }
            if(message.data[0] & (1 << can_msg::RIGHT_SIGNAL))
            {
                aux_input_state |= (1 << RIGHT_SIGNAL_BIT);
            }
            else
            {
                aux_input_state &= ~(1 << RIGHT_SIGNAL_BIT);
            }
            if(message.data[0] & (1 << can_msg::HAZARD_LIGHTS))
            {
                aux_input_state |= (1 << HAZARDS_BIT);
            }
            else
            {
                aux_input_state &= ~(1 << HAZARDS_BIT);
            }
    }
    else if ((message.id & (DEVICE_MASK|MESSAGE_MASK)) == (lcd_horn_msg.id() & (DEVICE_MASK|MESSAGE_MASK)))
    {
            if(message.data[0] & (1 << 0))
            {
                aux_input_state |= (1 << HORN_BIT);
            }
            else
            {
                aux_input_state &= ~(1 << HORN_BIT);
            }
    }
    else if ((message.id & (DEVICE_MASK|MESSAGE_MASK)) == (lcd_wipers_msg.id() & (DEVICE_MASK|MESSAGE_MASK)))
    {
            if(message.data[0] & (1 << 0))
            {
                aux_input_state |= (1 << WIPERS_BIT);
            }
            else
            {
                aux_input_state &= ~(1 << WIPERS_BIT);
            }
    }
    else if ((message.id & (DEVICE_MASK|MESSAGE_MASK)) == (lcd_headlights_msg.id() & (DEVICE_MASK|MESSAGE_MASK)))
    {
            if(message.data[0] & (1 << 0))
            {
                aux_input_state |= (1 << HEADLIGHTS_BIT);
            }
            else
            {
                aux_input_state &= ~(1 << HEADLIGHTS_BIT);
            }
    }    
    else if ((message.id & (DEVICE_MASK|MESSAGE_MASK)) == (brake_msg.id() & (DEVICE_MASK|MESSAGE_MASK)))
    {
            if(message.data[0])
            {
                aux_input_state |= (1 << BRAKE_BIT);
            }
            else
            {
                aux_input_state &= ~(1 << BRAKE_BIT);
            }
    }
    else if ((message.id & (DEVICE_MASK|MESSAGE_MASK)) == (lcd_headlights_msg.id() & (DEVICE_MASK|MESSAGE_MASK)))
    {
            if(message.data[0])
            {
                aux_input_state |= (1 << HEADLIGHTS_BIT);
            }
            else
            {
                aux_input_state &= ~(1 << HEADLIGHTS_BIT);
            }
    }
}

uint8_t check_brake(void)
{
    if(aux_input_state & (1 << BRAKE_BIT))
    {
        return(1);
    }
    return(0);
}

uint8_t check_left_signal(void)
{
    if(aux_input_state & (1 << LEFT_SIGNAL_BIT))
    {
        return(1);
    }
    return(0);
}

uint8_t check_right_signal(void)
{
    if(aux_input_state & (1 << RIGHT_SIGNAL_BIT))
    {
        return(1);
    }
    return(0);
}

uint8_t check_horn(void)
{
    if(aux_input_state & (1 << HORN_BIT))
    {
        return(1);
    }
    return(0);
}

uint8_t check_wipers(void)
{
    if(aux_input_state & (1 << WIPERS_BIT))
    {
        return(1);
    }
    return(0);
}

uint8_t check_hazards(void)
{
  if(aux_input_state & ( 1 << HAZARDS_BIT))
  {
    return(1);
  }
  return(0);
}

uint8_t check_headlights(void)
{
  if(aux_input_state & ( 1 << HEADLIGHTS_BIT))
  {
    return(1);
  }
  return(0);
}
