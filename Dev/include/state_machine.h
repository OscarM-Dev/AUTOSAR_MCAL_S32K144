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
typedef enum { SINGLE_CLICK, DOUBLE_CLICK, HOLD_CLICK } clicks;
typedef enum { BTN_1, BTN_2, BTN_3 } buttons;

//Structures
typedef struct _BtnsCtrl
{
    uint8 Button;  ///< Dio channel where the button is connected.
    uint8 State;   ///< State of the button state machine.
    uint8 Timer;   ///< 300ms timer ID,
} BtnsCtrl;

//Global data
extern BtnsCtrl stBtnMachine[3];

//Functions.
void DoubleClick_stMachine( void );

#endif