/**
 * @file Scheduler.c
 * @brief This file contains the definition of the functions of the round robin scheduler and the queue.
 *
 * The scheduler every tick checks the timer and task buffers and execute them according to their activation flag and
 * established timeout and periodicity, basically the execution is related to an specific function call.
 */

//Headers.
#include "Scheduler.h"

/**
 * @brief This function copies and pastes size bytes of data from src to dest.
 * 
 * @param Dest Void pointer to destiny, destiny base memory address.
 * @param Src Void pointer to source, source base memory address.
 * @param Size Number of bytes to copy and paste.
 */
void MemCopy( void *Dest, void *Src, uint32 Size ) {
    //local data.
    uint8 i = 0;
    uint8 *Ptr_dest = Dest; //Byte ptr in destiny base memory address.
    uint8 *Ptr_src = Src;   //Byte ptr in source base memory address.

    //Copying and pasting byte by byte.
    for ( i = 0; i < Size; i++ ) {
        *( Ptr_dest + i ) = *( Ptr_src + i );
    }
}

/**
 * @brief This function initialices the queue buffer control members.
 * 
 * @param SchedulerPtr Ptr to control struct instance, it allows the init of the members.
 */
void Scheduler_InitQueue( Scheduler_CtrlType *SchedulerPtr ) {
    //local data.
    uint8 i = 0;

    //Initializing members.
    for ( i = 0; i < SCHEDULER_QUEUES; i++ ) {
        SchedulerPtr->QueueHeads[i] = 0;
        SchedulerPtr->QueueTails[i] = 0;
        Bfx_ClrBit_u32u8( &SchedulerPtr->QueueFulls, i );
        Bfx_SetBit_u32u8( &SchedulerPtr->QueueEmpties, i );
    }
}

/**
 * @brief This function indicates the value of the specified status flag of a given queue.
 * 
 * @param Queue Queue ID.
 * @param Status Status flag of interest.
 * 
 * @return status Value of the queue flag of interest.
 * 
 * @note The queue ID must be valid.
 */
uint8 Scheduler_GetStatusQueue( QueueType Queue, uint8 Status ) {
    //local data.
    uint8 status = FALSE;

    //Verifying if the ID is valid.
    if ( ( Queue != 0 ) && ( Queue <= SchedulerConfig_Ptr->Queues ) ) { //Valid ID.      
        switch ( Status ) { //Checking status flag of interest.
            case SCHEDULER_QUEUE_EMPTY_STATUS: 
                status = Bfx_GetBit_u32u8_u8( SchedulerCtrl_Ptr->QueueEmpties, Queue - 1 ); 
            break;
            case SCHEDULER_QUEUE_FULL_STATUS:
                status = Bfx_GetBit_u32u8_u8( SchedulerCtrl_Ptr->QueueFulls, Queue - 1 ); 
            break;
            default:    //Invalid Status flag.
            break;
        }
    } 
    
    else {
        status = FALSE; //Invalid ID.
    }

    return status;
}

/**
 * @brief This function empties a given queue.
 * 
 * @param Queue Queue ID.
 * 
 * @return status Status of the operation, successfull 1 or not 0.
 * 
 * @note The queue ID must be valid.
 */
Std_ReturnType Scheduler_FlushQueue( QueueType Queue ) {
    //local data
    Std_ReturnType status = FALSE;

    //Verifying if the ID is valid.
    if ( ( Queue != 0 ) && ( Queue <= SchedulerConfig_Ptr->Queues ) ) { //Valid ID.   
        SchedulerCtrl_Ptr->QueueTails[ Queue - 1 ] = SchedulerCtrl_Ptr->QueueHeads[ Queue - 1 ];    //Emptying queue.
        Bfx_ClrBit_u32u8( &SchedulerCtrl_Ptr->QueueFulls, Queue - 1 );
        Bfx_SetBit_u32u8( &SchedulerCtrl_Ptr->QueueEmpties, Queue - 1 );
        status = TRUE;
    }

    else {  //Invalid ID.
        status = FALSE; 
    }

    return status;
}

/**
 * @brief This function writes a single data to a given queue.
 *
 * This function writes a single data to the queue if there´s an avalaible space 
 * if not it doesn´t modified the queue.
 * 
 * For achieving this it copies and pastes size bytes of info from data to the head element of the queue.
 * 
 * This operation uses the base memory address of both parameters, in case of the 
 * queue it uses the actual Head index for getting the memory address.
 * 
 * Once the writing operation is done it increments head by one.
 * 
 * It also determines wheter the queue is full or not after writing to it.
 * 
 * @param[in] Queue Queue ID.
 * @param[in] Data Void ptr to the data to be written, allows the access to the data.
 *
 * @retval status Status of the write operation, write operation was succesfull 1 or not 0.
 * 
 * @note The queue ID must be valid.
*/
Std_ReturnType Scheduler_WriteQueue( QueueType Queue, void *Data ) {
    //Local data
    Std_ReturnType status = FALSE;

     //Verifying if the ID is valid.
    if ( ( Queue != 0 ) && ( Queue <= SchedulerConfig_Ptr->Queues ) ) { //Valid ID.  
        uint32 base_add = ( uint32 ) SchedulerConfig_Ptr->QueuePtr[ Queue - 1 ].Buffer; //queue element 0 memory address.
        uint32 actual_add = 0;  //memory address of the actual queue element.

        //Verifying if the queue is availabe to be written.
        if ( Scheduler_GetStatusQueue( Queue, SCHEDULER_QUEUE_FULL_STATUS ) == FALSE ) {    //Spaces available.

            //Writing to array, copying and pasting size bytes of info from data to queue.
            actual_add = base_add + ( SchedulerCtrl_Ptr->QueueHeads[ Queue - 1 ] * SchedulerConfig_Ptr->QueuePtr[ Queue - 1 ].Size );
            MemCopy( ( void* )actual_add, Data, SchedulerConfig_Ptr->QueuePtr[ Queue - 1 ].Size );

            SchedulerCtrl_Ptr->QueueHeads[ Queue - 1 ]++;  //next value to be written.

            if ( SchedulerCtrl_Ptr->QueueHeads[ Queue - 1 ] > SchedulerConfig_Ptr->QueuePtr[ Queue - 1 ].Elements - 1 ) {
                SchedulerCtrl_Ptr->QueueHeads[ Queue - 1 ] = 0;    //Reseting write index.
            }

            if ( Scheduler_GetStatusQueue( Queue, SCHEDULER_QUEUE_EMPTY_STATUS ) == TRUE ) {
                Bfx_ClrBit_u32u8( &SchedulerCtrl_Ptr->QueueEmpties, Queue - 1 );    //Once data is written the queue is no longer empty.
            }

            //Verifying if queue is full with the last data written.
            if ( SchedulerCtrl_Ptr->QueueHeads[ Queue - 1 ] == SchedulerCtrl_Ptr->QueueTails[ Queue - 1 ] ) {
                Bfx_SetBit_u32u8( &SchedulerCtrl_Ptr->QueueFulls, Queue - 1 );
            }

            status = TRUE;  //Succesfull operation.
        }
    }

    else {  //Invalid ID.
        status = FALSE; 
    }

    return status;
}

/**
 * @brief This function reads a single data from a given queue.
 * 
 * This function reads a single data from the queue if it´s not empty if it´s empty it doesn´t read anything.
 * 
 * For achieving this it copies and pastes size bytes of info from the tail element of the queue to the data.
 * 
 * This operation uses the base memory address of both parameters, in case of the 
 * queue it uses the actual Tail index for getting the memory address.
 * 
 * Once the reading operation is done it increments tail by one.
 * 
 * The data read no longer exists in the queue.
 * 
 * It also determines wheter the queue is empty or not after reading from it.
 * 
 * @param[in] Queue Queue ID.
 * @param[in] Data Void ptr to the variable to put the read data from the queue, allows the read from the queue.
 *
 * @retval status Status of the read operation, read operation was succesfull 1 or not 0.
 * 
 * @note The queue ID must be valid.
*/
Std_ReturnType Scheduler_ReadQueue( QueueType Queue, void *Data ) {
    //Local data
    Std_ReturnType status = FALSE;

     //Verifying if the ID is valid.
    if ( ( Queue != 0 ) && ( Queue <= SchedulerConfig_Ptr->Queues ) ) { //Valid ID.
        uint32 base_add = ( uint32 ) SchedulerConfig_Ptr->QueuePtr[ Queue - 1 ].Buffer; //queue element 0 memory address.
        uint32 actual_add = 0;  //memory address of the actual queue element.     
        
        //Verifying if the queue has data available to be read.
        if ( Scheduler_GetStatusQueue( Queue, SCHEDULER_QUEUE_EMPTY_STATUS ) == FALSE ) { 
        
            //Reading from array, copying and pasting size bytes of info from queue to data.
            actual_add = base_add + ( SchedulerCtrl_Ptr->QueueTails[ Queue - 1 ] * SchedulerConfig_Ptr->QueuePtr[ Queue - 1 ].Size );
            MemCopy( Data, ( void* )actual_add, SchedulerConfig_Ptr->QueuePtr[ Queue - 1 ].Size );            
        
            SchedulerCtrl_Ptr->QueueTails[ Queue - 1 ]++;  //next value to be read.

            if ( SchedulerCtrl_Ptr->QueueTails[ Queue - 1 ] > SchedulerConfig_Ptr->QueuePtr[ Queue - 1 ].Elements - 1 )  {
                SchedulerCtrl_Ptr->QueueTails[ Queue - 1 ] = 0; //Reseting read index.
            }

            if ( Scheduler_GetStatusQueue( Queue, SCHEDULER_QUEUE_FULL_STATUS ) == TRUE ) {
                Bfx_ClrBit_u32u8( &SchedulerCtrl_Ptr->QueueFulls, Queue - 1 );  //Once data is read the queue is no longer full.
            }

            //Verifying if the queue is empty with the last data read.
            if ( SchedulerCtrl_Ptr->QueueTails[ Queue - 1 ] == SchedulerCtrl_Ptr->QueueHeads[ Queue - 1 ] ) {
                Bfx_SetBit_u32u8( &SchedulerCtrl_Ptr->QueueEmpties, Queue - 1 );
            }
            
            status = TRUE; //Succesfull operation.            
        }
    }

    else {  //Invalid ID.
        status = FALSE; 
    }

    return status;  
}

/**
 * @brief This function initialices the scheduler.
 *
 * This function initialices the control structure of the scheduler.
 * 
 * @param[in] SchedulerPtr Ptr to control struct instance, it allows the init of the members.
 *
 * @retval status Status of the operation, successfull 1 or not 0.
 *
 * @note The timeout and periodicity values are the initial values registered in the buffers.
 * @note The elapsed and count values are initialized to 0 and timeout values respectively.
 * @note All the flags are initialiced to the initial values registered in the buffers.
 */
Std_ReturnType Scheduler_Init( Scheduler_CtrlType *SchedulerPtr ) {
    //local data 
    Std_ReturnType status = TRUE;
    uint8 i = 0;

    //Initializing tasks related parameters.
    for ( i = 0; i < SCHEDULER_TASKS; i++ ) {
        SchedulerPtr->TaskPeriod[i] = SchedulerConfig_Ptr->TaskPtr[i].InitPeriod;   //Registering initial periodicity value of each task.
        SchedulerPtr->TaskElapsed[i] = 0;   //Initializing time follow up of each task.
        Bfx_PutBit_u32u8u8( &SchedulerPtr->TaskFlags, i, SchedulerConfig_Ptr->TaskPtr[i].InitFlag  );   //Registering initial flag of each task. 
    }

    //Initializing timers related parameters.
    for ( i = 0; i < SCHEDULER_TIMERS; i++ ) {
        SchedulerPtr->TimerTimeout[i] = SchedulerConfig_Ptr->TimerPtr[i].InitTimeout;   //Registering initial timeout value of each timer.
        SchedulerPtr->TimerCount[i] = SchedulerConfig_Ptr->TimerPtr[i].InitTimeout; //Initializing timer count of each timer.
        Bfx_PutBit_u32u8u8( &SchedulerPtr->TimerFlags, i, SchedulerConfig_Ptr->TimerPtr[i].InitFlag  );   //Registering initial flag of each timer.
    } 

    Scheduler_InitQueue( SchedulerPtr ); //Initializing queues related parameters.   

    return status;
}

/**
 * @brief This function stops a task.
 *
 * This function stops a registered task from being executed by disabling its start flag.
 *
 * @param[in] Task Task ID of the task to be stopped.
 *
 * @retval status Status of the operation, successfull 1 or not 0. 
 *
 * @note The task ID must be valid.
 */
Std_ReturnType Scheduler_StopTask( TaskType Task ) {
    //local data
    Std_ReturnType status = FALSE; 

    //Verifying if the task ID is valid.
    if ( ( Task != 0 ) && ( Task <= SchedulerConfig_Ptr->Tasks ) ) {    //valid ID
        Bfx_ClrBit_u32u8( &SchedulerCtrl_Ptr->TaskFlags, Task - 1 );
        status = TRUE;
    }

    else {
        status = FALSE; //invalid ID.
    }

    return status;
}

/**
 * @brief This function starts a task.
 *
 * This function starts the execution of a registered task by enabling its start flag.
 *
 * @param[in] Task Task ID of the task to be started.
 *
 * @retval status Status of the operation, successfull 1 or not 0. 
 *
 * @note The task ID must be valid.
 */
Std_ReturnType Scheduler_StartTask( TaskType Task ) {
    //local data
    Std_ReturnType status = FALSE;

    //Verifying if the task ID is valid.
    if ( ( Task != 0 ) && ( Task <= SchedulerConfig_Ptr->Tasks ) ) {    //valid ID
        Bfx_SetBit_u32u8( &SchedulerCtrl_Ptr->TaskFlags, Task - 1 );
        status = TRUE;
    }

    else {
        status = FALSE; //invalid ID.
    }
    
    return status;
}

/**
 * @brief This function changes the periodicity of a task.
 *
 * This function changes the periodicity of a registered task by modifying the period value of the task.
 *
 * @param[in] Task Task ID of the task to be modified.
 * @param[in] NewPeriod Periodicity value to be set.
 *
 * @retval status Status of the operation, successfull 1 or not 0. 
 *
 * @note The periodicity value must be >= tick and a multiple of tick.
 */
Std_ReturnType Scheduler_PeriodTask( TaskType Task, uint32 NewPeriod ){
    //local data.
    Std_ReturnType status = FALSE;

    //Verifying if the task ID is valid.
    if ( ( Task != 0 ) && ( Task <= SchedulerConfig_Ptr->Tasks ) ) {    //valid ID
        //Verifying if periodicity value is valid according to tick value. Period >= tick, period must be multiple of tick.
        if ( ( NewPeriod >= SchedulerConfig_Ptr->Tick ) && ( NewPeriod % SchedulerConfig_Ptr->Tick == 0 ) ) {   //valid periodicity.
            SchedulerCtrl_Ptr->TaskPeriod[ Task - 1 ] = NewPeriod;
            status = TRUE;
        }

        else {
            status = FALSE; //invalid periodicity.
        }
    }

    else {
        status = FALSE; //invalid ID.
    }

    return status;

}

/**
 * @brief This function starts a timer.
 *
 * This function starts the execution of a registered timer by enabling its start flag.
 * It also restarts the count value in case the timer has reach 0.
 *
 * @param[in] Timer Timer ID of the timer to be started.
 *
 * @retval status Status of the operation, successfull 1 or not 0. 
 *
 * @note The timer ID must be valid.
 */
Std_ReturnType Scheduler_StartTimer( TimerType Timer ) {
    //local data
    Std_ReturnType status = FALSE;

    //Verifying if the timer ID is valid.
    if ( ( Timer != 0 ) && ( Timer <= SchedulerConfig_Ptr->Timers ) ) { //valid ID
        Bfx_SetBit_u32u8( &SchedulerCtrl_Ptr->TimerFlags , Timer - 1 ); 

        //Verifying if timer has reach 0.
        if ( SchedulerCtrl_Ptr->TimerCount[ Timer - 1 ] == 0 ) {
             SchedulerCtrl_Ptr->TimerCount[ Timer - 1 ] = SchedulerCtrl_Ptr->TimerTimeout[ Timer - 1 ]; //restarting timer.
        }

        status = TRUE;
    }

    else {
        status = FALSE; //invalid ID.
    }

    return status;
}

/**
 * @brief This function stops a timer.
 *
 * This function stops a registered timer from being executed by disabling its start flag.
 *
 * @param[in] Timer Timer ID of the timer to be stopped.
 *
 * @retval status Status of the operation, successfull 1 or not 0. 
 *
 * @note The timer ID must be valid.
 */
Std_ReturnType Scheduler_StopTimer( TimerType Timer ) {
    //local data
    Std_ReturnType status = FALSE; 

    //Verifying if the timer ID is valid.
    if ( ( Timer != 0 ) && ( Timer <= SchedulerConfig_Ptr->Timers ) ) {//valid ID
        Bfx_ClrBit_u32u8( &SchedulerCtrl_Ptr->TimerFlags , Timer - 1 );
        status = TRUE;
    }

    else {
        status = FALSE;//invalid ID.
    }

    return status;
}

/**
 * @brief This function gets the count value of a timer.
 *
 * This function gets the actual counter value of a given timer.
 *
 * @param[in] Timer Timer ID of the timer to obtain count value.
 *
 * @retval counter_value Actual count value of timer, if operation was a success counter_value = n, otherwise 0. 
 *
 * @note The timer ID must be valid.
 */
uint32 Scheduler_GetTimer( TimerType Timer ) {
    //local data
    uint32 counter_value = 0;

    //Verifying if the timer ID is valid.
    if ( ( Timer != 0 ) && ( Timer <= SchedulerConfig_Ptr->Timers ) ) { //valid ID
        counter_value = SchedulerCtrl_Ptr->TimerCount[ Timer - 1 ];
    }

    else {
        counter_value = FALSE;  //invalid ID.
    }

    return counter_value;
}

/**
 * @brief This function changes the timeout of a timer.
 *
 * This function changes the timeout of a registered timer by modifying the timeout value of the timer.
 * It also resets the count value to the new timeout value and starts the timer.
 *
 * It can reload the timer regardless of the actual count value.
 * 
 * @param[in] Timer Timer ID of the timer to be modified.
 * @param[in] NewTimeout Timeout value to be set.
 *
 * @retval status Status of the operation, successfull 1 or not 0. 
 *
 * @note The timeout value must be >= tick and a multiple of tick.
 * @note The timer ID must be valid.
 */
Std_ReturnType Scheduler_ReloadTimer( TimerType Timer, uint32 NewTimeout ) {
    //local data
    Std_ReturnType status = FALSE;

    //Verifying if the timer ID is valid.
    if ( ( Timer != 0 ) && ( Timer <= SchedulerConfig_Ptr->Timers ) ) { //valid ID
        //Verifying if timeout value is valid according to tick value. timeout >= tick, timeout must be multiple of tick.
        if ( ( NewTimeout >= SchedulerConfig_Ptr->Tick ) && ( NewTimeout % SchedulerConfig_Ptr->Tick == 0 ) ) { //valid timeout.
            SchedulerCtrl_Ptr->TimerTimeout[ Timer - 1 ] = NewTimeout;
            SchedulerCtrl_Ptr->TimerCount[ Timer - 1 ]  = NewTimeout; //reseting timer to new reference value.
            Scheduler_StartTimer( Timer );
            status = TRUE;
        }

        else {
            status = FALSE; //Invalid timeout.
        }
     }

    else {
        status = FALSE; //invalid ID.
    }

    return status;
}

/**
 * @brief This function executes the scheduler.
 *
 * This function executes the tasks activated and registered in the buffer task as well as the timers activated and 
 * registered in the buffer timer according to the scheduler's tick (in terms of increments of the osif timer) and the periodicity or timeout 
 * of each task and timer in a cyclic way.
 *
 * @note At least 1 timer and 1 task needs to be register in order to the scheduler to work.
 * @note After a timer reaches 0 is important to restart, stop or reload that timer else it will still decrement --> risk of overflow.
 * @note It´s important that previous to using the scheduler the buffers for the timers and task must be correctly defined, specially their flags.
 */
void Scheduler_MainFunction( void ) {
    //local data
    uint32 tick_value = OsIf_MicrosToTicks( SchedulerConfig_Ptr->Tick * 1000, OSIF_COUNTER_SYSTEM );    //Translating tick value from us to counter increments.
    uint32 count_value = OsIf_GetCounter( OSIF_COUNTER_SYSTEM );    //First counter value capture.
    uint32 elapsed_time = 0;    //Time follow up for tick (timer increments).
    uint8 i = 0;

    while( 1 ) {    //tasks are executed during timeout.
        //Time follow up update for tick.
        elapsed_time += OsIf_GetElapsed( &count_value, OSIF_COUNTER_SYSTEM );   //time capture update
        
        //Verifying if a scheduler´s tick has occurred.
        if ( elapsed_time >= tick_value ) {          
            
            //Executing timers
            for ( i = 0; i < SchedulerConfig_Ptr->Timers; i++ ) {
                //Verifying execution of timer i
                if ( Bfx_GetBit_u32u8_u8( SchedulerCtrl_Ptr->TimerFlags, i ) == TRUE ) {    //timer activated
                    SchedulerCtrl_Ptr->TimerCount[i] -= SchedulerConfig_Ptr->Tick;    //decrement
                    
                    //Verifying execution of callback i
                    if ( Scheduler_GetTimer( i + 1 ) == 0 ) {   //timeout achieved.
                        SchedulerConfig_Ptr->TimerPtr[i].CallbackFunc();
                    }
                }
            }

            //Executing tasks.
            for ( i = 0; i < SchedulerConfig_Ptr->Tasks; i++ ) {
                //Verifying execution of task i
                if ( Bfx_GetBit_u32u8_u8( SchedulerCtrl_Ptr->TaskFlags, i ) == TRUE ) {
                    SchedulerCtrl_Ptr->TaskElapsed[i] += SchedulerConfig_Ptr->Tick;   //Time follow-up update for task i.
                
                    //Verifying the execution of callback i.
                    if ( SchedulerCtrl_Ptr->TaskElapsed[i] >= SchedulerCtrl_Ptr->TaskPeriod[i] ) {  //periodicity achieved.
                        SchedulerConfig_Ptr->TaskPtr[i].TaskFunc();
                        SchedulerCtrl_Ptr->TaskElapsed[i] = 0;  //Time follow-up reset for task i.
                    }
                }
            }

            elapsed_time = 0;   //Reset time follow up for tick.
        }
    }
}

