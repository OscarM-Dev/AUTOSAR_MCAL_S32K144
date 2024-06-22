/**
 * @file Callbacks.h
 * @brief This header contains the declaration of structures and functions 
 * for the timers and tasks to be used in the integration of the scheduler and queue.
 */

#ifndef CALLBACKS_H
#define CALLBACKS_H
//Headers.
#include "queue.h"
#include "Scheduler.h"
#include "State_machine.h"
#include "Dio.h"

//Structures.

/// @cond IGNORE
//Task´s functions.
void Task1_callback( void );
void Task2_callback( void );

//Timer´s callback functions.
void Timer1_callback( void );
void Timer2_callback( void );
/// @endcond

#endif