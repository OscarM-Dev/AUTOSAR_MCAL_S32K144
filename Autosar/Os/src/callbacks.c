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
 * This function executes the state machine every 50ms.
 */
void Task1_callback( void ) {
    DoubleClick_stMachine();
}

/**
 * @brief This is the callback function for the task 2.
 *
 * This function is called every 100ms and toggles a led according to the kind of click detected in the state machine.
 */
void Task2_callback( void ) {
    //Checking actual state.
    switch ( click ) {
        case SINGLE_CLICK: 
            Dio_FlipChannel( DioConf_DioChannel_PTC8 );
            click = NO_CLICK; 
        break;
        case DOUBLE_CLICK: 
            Dio_FlipChannel( DioConf_DioChannel_PTC9 );
            click = NO_CLICK; 
        break;
        case HOLD_CLICK: 
            Dio_FlipChannel( DioConf_DioChannel_PTC10 );
            click = NO_CLICK; 
        break;
        default: 
        break; //No click detected
    }
}

//Callbacks.
/**
 * @brief This is the callback function for the timer 1.
 *
 * This function is called when the timer counts to 300ms. This is for detecting the timeout in the state machine.
 */
void Timer1_callback( void ) {    
    Scheduler_StopTimer( SCHEDULER_TIMER1_ID );    //Stoping timer.
}

/**
 * @brief This is the callback function for the timer 2. 
 *
 */
void Timer2_callback( void ) {
    //Scheduler_StartTimer( SCHEDULER_TIMER2_ID );   //Restarting timer.
}