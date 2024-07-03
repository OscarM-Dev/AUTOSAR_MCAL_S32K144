/**
 * @file Callbacks.c
 * @brief This file contains the definition of functions 
 * for the timers and tasks to be used in the integration of the scheduler and queue.
 */

//Headers.
#include "callbacks.h"

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
 * This function is called every 100ms and gets the last event detected for each button for executing a certain action.
 * 
 * @note Button 1, Single click toggle only led0 (PTC8), double click prints ohms value of pot 1 main, hold click tone 1 for buzzer, release stops buzzer.
 * @note Button 2, Single click toggle only led1 (PTC9), double click prints ohms value of pot 2 main, hold click tone 2 for buzzer, release stops buzzer.
 * @note Button 3, Single click toggle only led2 (PTC10), double click prints ohms values of pot1 and pot 2 alter , hold click tone 3 for buzzer, release stops buzzer.
 * @note The tone of the buzzer is always the last activated.
 */
void Task2_callback( void ) {
    //local data.
    static boolean flags[3] = { TRUE, TRUE, TRUE }; //Flags for executing buzzer tones.
    uint8 i = 0;
    uint8 actual_event = 0; //Actual event of button.

    for ( i = 0; i < ButtonsControl_Ptr->Buttons; i++ ) { //Checking each button and its event detected.
        actual_event =  HwIoAb_Buttons_GetEvent( i );   //Obtaining event of actual button.
        
        if ( actual_event != HWIOAB_BTN_EVENT_IDLE ) {  //A click was detected.
            switch ( i ) {    
                case BTN_1: 
                    switch ( actual_event ) {
                        case HWIOAB_BTN_EVENT_SINGLE_CLICK:
                            HwIoAb_Leds_TurnToggle( HWIOAB_LEDS_0_ID );
                        break;
                        case HWIOAB_BTN_EVENT_DOUBLE_CLICK:
                            HwIoAb_Pots_GetValue( HWIOAB_POTS_MAIN_RESULTS );
                            SEGGER_RTT_printf( 0, "Pot 1 resistance value in ohms: %d\n", PotsControl_Ptr->Ohms_results_main[0] );
                        break;
                        case HWIOAB_BTN_EVENT_HOLD_CLICK:
                            if ( flags[i]  == TRUE ) {  //Just executes the first time it was detected.
                                HwIoAb_Buzzer_Beep( HWIOAB_BUZZER_TONE_1 );
                                flags[i] = FALSE;
                            }
                        break;
                        case HWIOAB_BTN_EVENT_RELEASE:
                            HwIoAb_Buzzer_Stop();
                            flags[i] = TRUE;
                        break;
                        default:    //Null event.
                        break;
                    }
                break;
            
                case BTN_2:
                    switch ( actual_event ) {
                        case HWIOAB_BTN_EVENT_SINGLE_CLICK:
                            HwIoAb_Leds_TurnToggle( HWI0AB_LEDS_1_ID );
                        break;
                        case HWIOAB_BTN_EVENT_DOUBLE_CLICK:
                            HwIoAb_Pots_GetValue( HWIOAB_POTS_MAIN_RESULTS );
                            SEGGER_RTT_printf( 0, "Pot 2 resistance value in ohms: %d\n", PotsControl_Ptr->Ohms_results_main[1] );
                        break;
                        case HWIOAB_BTN_EVENT_HOLD_CLICK:
                            if ( flags[i]  == TRUE ) {  //Just executes the first time it was detected.
                                HwIoAb_Buzzer_Beep( HWIOAB_BUZZER_TONE_2 );
                                flags[i] = FALSE;
                            }
                        break;
                        case HWIOAB_BTN_EVENT_RELEASE:
                            HwIoAb_Buzzer_Stop();
                            flags[i] = TRUE;
                        break;
                        default:    //Null event.
                        break;
                    }
                break;

                case BTN_3:
                        switch ( actual_event ) {
                            case HWIOAB_BTN_EVENT_SINGLE_CLICK:
                                HwIoAb_Leds_TurnToggle( HWIOAB_LEDS_2_ID );
                            break;
                            case HWIOAB_BTN_EVENT_DOUBLE_CLICK:
                                HwIoAb_Pots_GetAltValue( HWIOAB_POTS_ALTER_RESULTS );
                                SEGGER_RTT_printf( 0, "Pot 1 resistance value in ohms: %d, Pot 2 resistance value in ohms: %d\n", 
                                    PotsControl_Ptr->Ohms_results_alter[0], PotsControl_Ptr->Ohms_results_alter[1] );
                            break;
                            case HWIOAB_BTN_EVENT_HOLD_CLICK:
                                if ( flags[i]  == TRUE ) {  //Just executes the first time it was detected.
                                    HwIoAb_Buzzer_Beep( HWIOAB_BUZZER_TONE_3 );
                                    flags[i] = FALSE;
                                }
                            break;
                            case HWIOAB_BTN_EVENT_RELEASE:
                                HwIoAb_Buzzer_Stop();
                                flags[i] = TRUE;
                            break;
                            default:    //Null event.
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