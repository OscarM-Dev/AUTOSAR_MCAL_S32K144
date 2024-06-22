/**
 * @file State_machine.h
 * @brief This header contains the declaration of the functions and structures related to the double click state machine.
 * 
 * 
 */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

//Headers
#include "StandardTypes.h"
#include "Scheduler.h"
#include "Dio.h"

//Enums 
typedef enum { IDLE, SINGLE_PRESS, SINGLE_RELEASE, DOUBLE_PRESS, HOLD } states;
typedef enum { NO_CLICK, SINGLE_CLICK, DOUBLE_CLICK, HOLD_CLICK } clicks;

//Global data
extern uint8 click;

//Functions.
void DoubleClick_stMachine( void );

#endif