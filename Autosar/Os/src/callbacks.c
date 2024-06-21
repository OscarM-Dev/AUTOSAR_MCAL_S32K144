/**
 * @file Callbacks.c
 * @brief This file contains the definition of functions 
 * for the timers and tasks to be used in the integration of the scheduler and queue.
 */

//Headers.
#include "Callbacks.h"

//Tasks.
/**
 * @brief This is the callback function for the task 1.
 *
 * This function toggles a led every 100ms.
 */
void Task1_callback( void ) {
    Dio_FlipChannel( DioConf_DioChannel_PTC8 );
}

/**
 * @brief This is the callback function for the task 2.
 *
 * This function toggles a led every 200ms.
 */
void Task2_callback( void ) {
    Dio_FlipChannel( DioConf_DioChannel_PTC9 );
}

//Callbacks.
/**
 * @brief This is the callback function for the timer 1.
 *
 * This function toggles a led every 300ms.
 */
void Timer1_callback( void ) {    
    Dio_FlipChannel( DioConf_DioChannel_PTC10 );
    Scheduler_StartTimer( SCHEDULER_TIMER1_ID );    //Restarting timer.
}

/**
 * @brief This is the callback function for the timer 2. 
 *
 * This function toggles a led every 400ms.
 */
void Timer2_callback( void ) {
    Dio_FlipChannel( DioConf_DioChannel_PTC11 );
    Scheduler_StartTimer( SCHEDULER_TIMER2_ID );   //Restarting timer.
}