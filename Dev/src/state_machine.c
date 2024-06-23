/**
 * @file State_machine.c
 * @brief This file contains the definition of the functions related to the double click state machine.
 * 
 * This operation is based on the flow chart diagram.
 * 
 * @note This function is called every 50ms.
 * @note Remember that the 300ms count is for detecting a hold click when the button is pressed 1 and 2 times.
 * @note Each button has its own dedicated timer.
 * @note Button 1, Single click toggle only led0 (PTC8), double click toggle only led1 (PTC9), hold click toggle both leds (PTC8-9).
 * @note Button 2, Single click toggle only led2 (PTC10), double click toggle only led3 (PTC11), hold click toggle both leds (PTC10-11).
 * @note Button 3, Single click toggle only led5 (PTC13), double click toggle only led6 (PTC14), hold click toggle both leds (PTC13-14).
 * 
 */

//Headers
#include "state_machine.h"

/**
 * @brief This function is the double click state machine.
 * 
 */
void DoubleClick_stMachine( void ) {
    //local data
    uint8 i = 0;
    QueueMessage Message_write; //Message to write in the queue1.

    for ( i = 0; i < 3; i++ ) { //Executing state machine for each button.
        switch( stBtnMachine[i].State ) {
            case IDLE :
                //Checks if the button is pressed.
                if ( Dio_ReadChannel( stBtnMachine[i].Button ) == STD_LOW ) {
                    Scheduler_ReloadTimer( stBtnMachine[i].Timer, SCHEDULER_TIMER_TIMEOUT_300MS );   //Starting timer 300ms.
                    stBtnMachine[i].State = SINGLE_PRESS;
                }
            break;
        
            case SINGLE_PRESS :
                //Checking timer timeout.
                if ( Scheduler_GetTimer( stBtnMachine[i].Timer ) == 0 ) {
                    stBtnMachine[i].State = HOLD;
                }

                //Checking if the button is released.
                if ( Dio_ReadChannel( stBtnMachine[i].Button ) == STD_HIGH ) {
                    stBtnMachine[i].State = SINGLE_RELEASE;
                }
            break;
        
            case SINGLE_RELEASE :
                //Checking timer timeout.
                if ( Scheduler_GetTimer( stBtnMachine[i].Timer ) == 0 ) { //Single click
                    //Writing to the queue1 click detected.
                    Message_write.Button = i;
                    Message_write.Click = SINGLE_CLICK;
                    Scheduler_WriteQueue( SCHEDULER_QUEUE1_ID, &Message_write );
                    stBtnMachine[i].State = IDLE;
                }

                //Checks if the button is pressed.
                if ( Dio_ReadChannel( stBtnMachine[i].Button  ) == STD_LOW ) {
                    Scheduler_ReloadTimer( stBtnMachine[i].Timer, SCHEDULER_TIMER_TIMEOUT_300MS );   //Starting timer 300ms.
                    stBtnMachine[i].State = DOUBLE_PRESS;
                }
            break;
        
            case DOUBLE_PRESS :
                //Checking timer timeout.
                if ( Scheduler_GetTimer( stBtnMachine[i].Timer ) == 0 ) {
                    stBtnMachine[i].State = HOLD;
                }

                //Checking if the button is released.
                if ( Dio_ReadChannel( stBtnMachine[i].Button ) == STD_HIGH ) {//Double click.
                    //Writing to the queue1 click detected.
                    Message_write.Button = i;
                    Message_write.Click = DOUBLE_CLICK;
                    Scheduler_WriteQueue( SCHEDULER_QUEUE1_ID, &Message_write );
                    stBtnMachine[i].State = IDLE;
                }
            break;
        
            case HOLD ://Hold click.
                //Checking if the button is released.
                if ( Dio_ReadChannel( stBtnMachine[i].Button ) == STD_HIGH ) {
                    //Writing to the queue1 click detected.
                    Message_write.Button = i;
                    Message_write.Click = HOLD_CLICK;
                    Scheduler_WriteQueue( SCHEDULER_QUEUE1_ID, &Message_write );
                    stBtnMachine[i].State = IDLE;
                }
            break;
        
            default :
            break;
        }
    }
}
