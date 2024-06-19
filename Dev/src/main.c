/**
 * @file main.c
 * @brief This file contains the main function of the project
 * @author Modular-MX
 * 
 * This file contains the main function of the project, it is the entry point of the program
 * it contains a simple program that toggles a pin of the microcontroller
 * 
 * This is the template of the migrated scheduler for the MCU using 2 tasks and timers.
*/

//Header.
#include "Mcu.h"
#include "Port.h"
#include "Dio.h"
#include "OsIf.h"
#include "Mcu_init.h"
#include "queue.h"
#include "scheduler_t.h"
#include "callbacks.h"

//Global data
Sched_Scheduler Sche;///< Scheduler control structure instance.

/**
 * @brief This is the main function of the project
 * 
 * This is the main function of the project, it is the entry point of the program
 * 
 * @return Always zero
*/
int main( void ) {
    //local data.
    Sched_Task tasks[ n_tasks_h ];//Task buffer.
    Sched_Timer timers[ n_timers_h ];//Timer buffer.

    EcuM_Init();//MCU configuration.
    
    //Scheduler configuration.
    //Initializing scheduler.
    Sched_initScheduler( &Sche, n_tasks_c, n_tasks_h, TICK, tasks, n_timers_c, n_timers_h, timers );

    //Registering and activating tasks.
    Sched_registerTask( &Sche, Task1_init , Task1_callback, 100 );//Task 1.
    Sched_registerTask( &Sche, Task2_init, Task2_callback, 200 );//Task 2.
    Sched_startTask( &Sche, 1 );
    Sched_startTask( &Sche, 2 );

    //Registering and activating timers.
    Sched_registerTimer( &Sche, 300, Timer1_callback );//Timer 1.
    Sched_registerTimer( &Sche, 400, Timer2_callback );//Timer 2.
    Sched_startTimer( &Sche, 1 );
    Sched_startTimer( &Sche, 2 );

    while( 1u )
    {
        Sched_startScheduler( &Sche );//Starting scheduler.
    }

    return 0u;
}

