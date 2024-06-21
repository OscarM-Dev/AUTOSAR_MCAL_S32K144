/**
 * @file state_machine.h
 * @brief This header contains the declaration of the functions and structures related to the double click state machine.
 * 
 * 
 */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

//Headers
#include "StandardTypes.h"
//#include "Gpt.h"
#include "Dio.h"

//Macros.
#define FTM0CH0_V 3125 ///< Counter reference value for 50 ms.
#define FTM0CH1_V 18750 ///< Counter reference value for 300 ms.


//Enums 
typedef enum { IDLE, SINGLE_PRESS, SINGLE_RELEASE, DOUBLE_PRESS, HOLD } states;

//Global data.
//extern uint8 state;

//Functions.
void DoubleClick_stMachine( void );

#endif