/**
 * @file callbacks.h
 * @brief This header contains the declaration of structures and functions 
 * for the timers and tasks to be used in the integration of the scheduler and queue.
 */

#ifndef CALLBACKS_H
#define CALLBACKS_H
//Headers.
#include <stdint.h>
#include "queue.h"
#include "scheduler_t.h"
#include "Dio.h"

//Structures.

/// @cond IGNORE
//Global data.
extern Sched_Scheduler Sche;

//Task´s functions.
void Task1_init( void );
void Task2_init( void );
void Task1_callback( void );
void Task2_callback( void );

//Timer´s callback functions.
void Timer1_callback( void );
void Timer2_callback( void );
/// @endcond

#endif