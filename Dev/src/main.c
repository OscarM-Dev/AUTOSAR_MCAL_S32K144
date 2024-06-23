/**
 * @file main.c
 * @brief This file contains the main function of the project
 * 
 * This file executes the double click state machine using the scheduler.
*/

//Header.
#include "Mcu_init.h"
#include "Scheduler.h"
#include "State_machine.h"

//Global data.
QueueMessage Queue1[ SCHEDULER_QUEUE1_ELEMENTS ];
BtnsCtrl stBtnMachine[3] = {
    {   //Button 1.
        .Button = DioConf_DioChannel_PTE12,
        .State = IDLE,
        .Timer = SCHEDULER_TIMER1_ID
    },
    {   //Button 2.
        .Button = DioConf_DioChannel_PTD17,
        .State = IDLE,
        .Timer = SCHEDULER_TIMER2_ID
    },
    {   //Button 3.
        .Button = DioConf_DioChannel_PTA12,
        .State = IDLE,
        .Timer = SCHEDULER_TIMER3_ID
    }
};

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

