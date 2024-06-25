/**
 * @file main.c
 * @brief This file contains the main function of the project
 * 
 * This file executes the double click state machine using the scheduler.
*/

//Header.
#include "Mcu_init.h"
#include "Scheduler.h"

//Global data.
QueueMessage Queue1[ SCHEDULER_QUEUE1_ELEMENTS ];

/**
 * @brief This is the main function of the project
 * 
 * This is the main function of the project, it is the entry point of the program
 * 
 * @return Always zero
*/
int main( void ) {

    EcuM_Init();    //MCU configuration.
    Scheduler_Init( &Scheduler_Control );   //Initializing control structure for scheduler.

    while( 1u ) {
        Scheduler_MainFunction();
    }

    return 0u;
}

