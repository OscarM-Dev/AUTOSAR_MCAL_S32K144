/**
 * @file main.c
 * @brief This file contains the main function of the project
 * 
 * This file contains
*/

//Header.
#include "mcu_init.h"
#include "gpt_notifications.h"

//Global data
uint8 state;

/**
 * @brief This is the main function of the project
 * 
 * This is the main function of the project, it is the entry point of the program
 * 
 * @return Always zero
*/
int main( void ) {

    EcuM_Init();//MCU configuration.
    Gpt_StartTimer( GptConf_GptChannelConfiguration_Gpt_Ftm0_ch0, FTM0CH0_V );//Starting timer 50ms.

    while( 1u ) {
    
    }

    return 0u;
}

