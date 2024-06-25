/**
 * @file HwIoAb_0_Buttons.c
 * @brief This file contains the definition of the functions related to 
 * the IO hardware abstraction for the buttons on board.
 * 
 */

//Headers
#include "HwIoAb_0_Buttons.h"

//Functions
/**
 * @brief This function initialices the control structure for the double click state machine. 
 * 
 * @param Buttons_Config Pointer to configuration array.
 * 
 */
void HwIoAb_Buttons_Init( const HwIoAb_Buttons_Config *Buttons_Config ) {
    //local data 
    uint8 i = 0;

    //Initializing members.
    ButtonsControl_Ptr->Buttons = HWIOAB_BUTTONS_MAX;
    ButtonsControl_Ptr->ButtonsConfig_Ptr = Buttons_Config;

    for ( i = 0; i < ButtonsControl_Ptr->Buttons; i++ ) {   //All button states and events to IDLE.
        ButtonsControl_Ptr->States[i] = HWIOAB_BTN_STATE_IDLE;
        ButtonsControl_Ptr->Events[i] = HWIOAB_BTN_EVENT_IDLE;
    }
}

/**
 * @brief This function indicates the last event detected in a button.
 * 
 * @param Button Button ID. 
 * @return event Last event detected in button.
 * 
 * @note The Button ID must be valid.
 */
uint8 HwIoAb_Buttons_GetEvent( uint8 Button ) {
    //local data.
    uint8 event = 0;

    //Verifying if ID is valid
    if ( ( Button != 0 ) && ( Button <= ButtonsControl_Ptr->Buttons ) ) {
        event = ButtonsControl_Ptr->Events[ Button - 1 ]; //Obtaining last event.
        ButtonsControl_Ptr->Events[ Button - 1 ] = HWIOAB_BTN_EVENT_IDLE;   //Clearing event of button.
    }

    return event;
}

/**
 * @brief This function is the double click state machine.
 * 
 * @note This function is called every HWIOAB_BUTTONS_PERIOD.
 * @note Remember that the timeout count is for detecting a hold click when the button is pressed 1 and 2 times.
 * @note Each button has its own dedicated count.
 * 
 */
void HwIoAb_Buttons_MainFunction( void ) {
    //local data
    static boolean count_flags[ HWIOAB_BUTTONS_MAX ] = { FALSE, FALSE, FALSE };
    static uint32 count[ HWIOAB_BUTTONS_MAX ] = { 0, 0, 0 };
    uint8 i = 0;
    boolean active = FALSE;
    
    //Counting in every function call
    for ( i = 0; i < ButtonsControl_Ptr->Buttons; i++ ) {   //Counting for each button.
        if ( count_flags[i] == TRUE ) {
            count[i] += HWIOAB_BUTTONS_PERIOD;

            if ( count[i] == ButtonsControl_Ptr->ButtonsConfig_Ptr[i].Timeout ) {   //Once reaching timeout count stops.
                count_flags[i] = FALSE;
            }
        }
    }

    for ( i = 0; i < ButtonsControl_Ptr->Buttons; i++ ) { //Executing state machine for each button.
        
        //Defining the active value according to the active status.
        if ( ButtonsControl_Ptr->ButtonsConfig_Ptr[i].Active == HWIOAB_BTN_ACTIVE_HIGH ) {  //Pull down.
            active = STD_HIGH;
        }

        else {  //Pull up.
            active = STD_LOW;
        }
        
        switch( ButtonsControl_Ptr->States[i] ) {
            case HWIOAB_BTN_STATE_IDLE :
                //Checks if the button is pressed.
                if ( Dio_ReadChannel( ButtonsControl_Ptr->ButtonsConfig_Ptr[i].Button ) == active ) {
                    count_flags[i] = TRUE;  //Starting count.
                    count[i] = 0;   //Restarting count.
                    ButtonsControl_Ptr->States[i] = HWIOAB_BTN_STATE_SINGLE_PRESS;
                }
            break;
        
            case HWIOAB_BTN_STATE_SINGLE_PRESS :
                //Checking count timeout.
                if ( count[i] == ButtonsControl_Ptr->ButtonsConfig_Ptr[i].Timeout ) {
                    ButtonsControl_Ptr->States[i] = HWIOAB_BTN_STATE_HOLD_PRESS;
                }

                //Checking if the button is released.
                if ( Dio_ReadChannel( ButtonsControl_Ptr->ButtonsConfig_Ptr[i].Button ) == !active ) {
                    ButtonsControl_Ptr->States[i] = HWIOAB_BTN_STATE_RELEASE;
                }
            break;
        
            case HWIOAB_BTN_STATE_RELEASE :
                //Checking count timeout.
                if ( count[i] == ButtonsControl_Ptr->ButtonsConfig_Ptr[i].Timeout ) { //Single click
                    ButtonsControl_Ptr->Events[i] = HWIOAB_BTN_EVENT_SINGLE_CLICK;
                    ButtonsControl_Ptr->States[i] = HWIOAB_BTN_STATE_IDLE;
                }

                //Checks if the button is pressed.
                if ( Dio_ReadChannel( ButtonsControl_Ptr->ButtonsConfig_Ptr[i].Button ) == active ) {
                    count_flags[i] = TRUE;  //Starting count.
                    count[i] = 0;   //Restarting count.
                    ButtonsControl_Ptr->States[i] = HWIOAB_BTN_STATE_DOUBLE_PRESS;
                }
            break;
        
            case HWIOAB_BTN_STATE_DOUBLE_PRESS :
                //Checking count timeout.
                if ( count[i] == ButtonsControl_Ptr->ButtonsConfig_Ptr[i].Timeout ) {
                    ButtonsControl_Ptr->States[i] = HWIOAB_BTN_STATE_HOLD_PRESS;
                }

                //Checking if the button is released.
                if ( Dio_ReadChannel( ButtonsControl_Ptr->ButtonsConfig_Ptr[i].Button ) == !active ) {  //Double click.
                    ButtonsControl_Ptr->Events[i] = HWIOAB_BTN_EVENT_DOUBLE_CLICK;
                    ButtonsControl_Ptr->States[i] = HWIOAB_BTN_STATE_IDLE;
                }
            break;
        
            case HWIOAB_BTN_STATE_HOLD_PRESS :  //Hold click.
                //Checking if the button is released.
                if ( Dio_ReadChannel( ButtonsControl_Ptr->ButtonsConfig_Ptr[i].Button ) == !active ) {
                    ButtonsControl_Ptr->Events[i] = HWIOAB_BTN_EVENT_HOLD_CLICK;
                    ButtonsControl_Ptr->States[i] = HWIOAB_BTN_STATE_IDLE;
                }
            break;
        
            default :
            break;
        }
    }
}
