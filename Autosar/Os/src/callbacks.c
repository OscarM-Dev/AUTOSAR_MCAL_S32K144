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
    HwIoAb_Buttons_MainFunction();
}

/**
 * @brief This is the callback function for the task 2.
 *
 * This function is called every 100ms and gets the last event detected for each button 
 * to toggle the corresponding led.
 * 
 * @note Button 1, Single click toggle only led0 (PTC8), double click toggle only led1 (PTC9), hold click toggle both leds (PTC8-9).
 * @note Button 2, Single click toggle only led2 (PTC10), double click toggle only led3 (PTC11), hold click toggle both leds (PTC10-11).
 * @note Button 3, Single click toggle only led4 (PTC13), double click toggle only led5 (PTC14), hold click toggle both leds (PTC13-14).
 * 
 */
void Task2_callback( void ) {
    //local data.
    uint8 i = 0;
    uint8 actual_event = 0; //Actual event of button.

    for ( i = 0; i < ButtonsControl_Ptr->Buttons; i++ ) { //Checking each button and its event detected.
        actual_event =  HwIoAb_Buttons_GetEvent( i + 1 );   //Obtaining event of actual button.
        if ( actual_event != HWIOAB_BTN_EVENT_IDLE ) {  //A click was detected.
            switch ( i ) {    
                case BTN_1: 
                    switch ( actual_event ) {
                        case HWIOAB_BTN_EVENT_SINGLE_CLICK:
                            HwIoAb_Leds_TurnToggle( HWIOAB_LEDS_0_ID );
                        break;
                        case HWIOAB_BTN_EVENT_DOUBLE_CLICK:
                            HwIoAb_Leds_TurnToggle( HWI0AB_LEDS_1_ID );
                        break;
                        case HWIOAB_BTN_EVENT_HOLD_CLICK:
                            HwIoAb_Leds_TurnToggle( HWIOAB_LEDS_0_ID );
                            HwIoAb_Leds_TurnToggle( HWI0AB_LEDS_1_ID );
                        break;
                        default:    //No click.
                        break;
                    }
                break;
            
                case BTN_2:
                    switch ( actual_event ) {
                        case HWIOAB_BTN_EVENT_SINGLE_CLICK:
                            HwIoAb_Leds_TurnToggle( HWIOAB_LEDS_2_ID );
                        break;
                        case HWIOAB_BTN_EVENT_DOUBLE_CLICK:
                            HwIoAb_Leds_TurnToggle( HWIOAB_LEDS_3_ID );
                        break;
                        case HWIOAB_BTN_EVENT_HOLD_CLICK:
                            HwIoAb_Leds_TurnToggle( HWIOAB_LEDS_2_ID );
                            HwIoAb_Leds_TurnToggle( HWIOAB_LEDS_3_ID );
                        break;
                        default:    //No click.
                        break;
                    }
                break;

                case BTN_3:
                        switch ( actual_event ) {
                            case HWIOAB_BTN_EVENT_SINGLE_CLICK:
                                HwIoAb_Leds_TurnToggle( HWIOAB_LEDS_4_ID );
                            break;
                            case HWIOAB_BTN_EVENT_DOUBLE_CLICK:
                                HwIoAb_Leds_TurnToggle( HWIOAB_LEDS_5_ID );
                            break;
                            case HWIOAB_BTN_EVENT_HOLD_CLICK:
                               HwIoAb_Leds_TurnToggle( HWIOAB_LEDS_4_ID );
                               HwIoAb_Leds_TurnToggle( HWIOAB_LEDS_5_ID );
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
 */
void Timer1_callback( void ) {    
}

/**
 * @brief This is the callback function for the timer 2.
 *
 */
void Timer2_callback( void ) {    
}

/**
 * @brief This is the callback function for the timer 3.
 *
 */
void Timer3_callback( void ) {    
}