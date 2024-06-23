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
 * This function is called every 100ms and reads the messagges from the queue
 * in order to toggle the leds according to the btn and its corresponding click detected.
 */
void Task2_callback( void ) {
    //local data.
    uint8 i = 0;
    QueueMessage Message_read;

    //Reading the queue if there are messages availables.
    for ( i = 0; i < 3; i++ ) { //Checking each button and its click detected.
        if ( Scheduler_GetStatusQueue( SCHEDULER_QUEUE1_ID, SCHEDULER_QUEUE_EMPTY_STATUS ) == FALSE ) {
            Scheduler_ReadQueue( SCHEDULER_QUEUE1_ID, &Message_read );

            switch ( Message_read.Button ) {    
                case BTN_1: 
                    switch ( Message_read.Click ) {
                        case SINGLE_CLICK:
                            Dio_FlipChannel( DioConf_DioChannel_PTC8 );
                        break;
                        case DOUBLE_CLICK:
                            Dio_FlipChannel( DioConf_DioChannel_PTC9 );
                        break;
                        case HOLD_CLICK:
                            Dio_FlipChannel( DioConf_DioChannel_PTC8 );
                            Dio_FlipChannel( DioConf_DioChannel_PTC9 );
                        break;
                        default:    //No click.
                        break;
                    }
                break;
            
                case BTN_2:
                    switch ( Message_read.Click ) {
                        case SINGLE_CLICK:
                            Dio_FlipChannel( DioConf_DioChannel_PTC10 );
                        break;
                        case DOUBLE_CLICK:
                            Dio_FlipChannel( DioConf_DioChannel_PTC11 );
                        break;
                        case HOLD_CLICK:
                            Dio_FlipChannel( DioConf_DioChannel_PTC10 );
                            Dio_FlipChannel( DioConf_DioChannel_PTC11 );
                        break;
                        default:    //No click.
                        break;
                    }
                break;

                case BTN_3:
                        switch ( Message_read.Click ) {
                            case SINGLE_CLICK:
                                Dio_FlipChannel( DioConf_DioChannel_PTC13 );
                            break;
                            case DOUBLE_CLICK:
                                Dio_FlipChannel( DioConf_DioChannel_PTC14 );
                            break;
                            case HOLD_CLICK:
                                Dio_FlipChannel( DioConf_DioChannel_PTC13 );
                                Dio_FlipChannel( DioConf_DioChannel_PTC14 );
                            break;
                            default:    //No click.
                            break;
                        }
                break;

                default:    //Invalid button.
                break;
            }
        }
    }
}

//Callbacks.
/**
 * @brief This is the callback function for the timer 1.
 *
 * This function is called when the timer counts to 300ms. 
 * This is for detecting the timeout for the button 1 in the state machine.
 */
void Timer1_callback( void ) {    
    Scheduler_StopTimer( SCHEDULER_TIMER1_ID );    //Stoping timer.
}

/**
 * @brief This is the callback function for the timer 2.
 *
 * This function is called when the timer counts to 300ms. 
 * This is for detecting the timeout for the button 2 in the state machine.
 */
void Timer2_callback( void ) {    
    Scheduler_StopTimer( SCHEDULER_TIMER2_ID );    //Stoping timer.
}

/**
 * @brief This is the callback function for the timer 3.
 *
 * This function is called when the timer counts to 300ms. 
 * This is for detecting the timeout for the button 3 in the state machine.
 */
void Timer3_callback( void ) {    
    Scheduler_StopTimer( SCHEDULER_TIMER3_ID );    //Stoping timer.
}