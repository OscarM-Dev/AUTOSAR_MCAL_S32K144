/**
 * @file gpt_notifications.c
 * @brief This file contains the definition of the notifications of the gpt driver.
 * 
 */

//Headers
#include "gpt_notifications.h"

//Funtions
/**
 * @brief This is the notification for Gpt_Ftm0_ch0.
 * This function executes the state machine every 50ms
 */
void Ftm0_ch0_callback( void ) {
    DoubleClick_stMachine();//Executing state machine.
}

/**
 * @brief This is the notification for Gpt_Ftm0_ch1.
 */
void Ftm0_ch1_callback( void ) {
}


