/**
 * @file scheduler_t.c
 * @brief This file contains the definition of the functions of the round robin scheduler.
 *
 * The scheduler every tick checks the timer and task buffers and execute them according to their activation flag and
 * established timeout and periodicity, basically the execution is related to an specific function call.
 */

//Headers.
#include "scheduler_t.h"
#include <stdio.h>

/**
 * @brief This function initialices the scheduler.
 *
 * This function initialices the control structure of the scheduler, for achieving this it checks the relation beetween 
 * the number of timers and tasks created with the number of tasks and timers to handle. 
 *
 * @param[in] scheduler Ptr to control struct instance, it allows the init of the members.
 * @param[in] n_tasks_c Number of tasks created.
 * @param[in] n_tasks_h Number of tasks to handle, it determines the task buffer size.
 * @param[in] tick Time monitoring interval value for the scheduler.
 * @param[in] buffer_ta Ptr to buffer of tasks.
 * @param[in] n_timers_c Number of timers created.
 * @param[in] n_timers_h Number of timers to handle, it determines the timer buffer size.
 * @param[in] buffer_ti Ptr to buffer of timers.
 *
 * @retval status Status of the operation, successfull 1 or not 0.
 *
 * @note The number of timers or tasks to handle must be <= that the number of tasks or timers created.
 */
uint8_t Sched_initScheduler( Sched_Scheduler *scheduler, uint8_t n_tasks_c, uint8_t n_tasks_h, uint32_t tick, Sched_Task *buffer_ta, uint8_t n_timers_c, uint8_t n_timers_h, Sched_Timer *buffer_ti ) {
    //local data 
    uint8_t status = true;
    uint8_t i = 0;
    
    //Initializing.
    scheduler->tasksCount = n_tasks_c;
    scheduler->tick = tick;
    scheduler->taskPtr = buffer_ta;

    //Verifying if number of tasks to handle is correct.
    if ( n_tasks_h <= scheduler->tasksCount ) {//valid number.
        scheduler->tasks = n_tasks_h;
        status  = status && true;
    }

    else {//invalid number.
        status = status && false;
    }
    
    scheduler->timersCount = n_timers_c;
    scheduler->timerPtr = buffer_ti;

    //Verifying if number of timers to handle is correct.
    if ( n_timers_h <= scheduler->timersCount ) {//valid number.
        scheduler->timers = n_timers_h;
        status  = status && true;
    }

    else {//invalid number.
        status = status && false;
    }
    
    //Initializing start flags for tasks and timers.
    scheduler->tasks_startFlags = 0;
    scheduler->timers_startFlags = 0;

    return status;
}

/**
 * @brief This function initialices the buffer of timers or tasks.
 *
 * This function initialices the buffer specified in buffer, it basically sets the memory address of the 
 * function pointer to NULL of all the elements of the buffer.
 *
 * @param[in] scheduler Ptr to control struct instance, it allows the initialization of the buffer.
 * @param[in] buffer Indicator of which buffer to initialice, 1 = timer buffer and 2 = task buffer.
 *
 * @retval status Status of the operation, successfull 1 or not 0. 
 *
 * @note This function can be used when a new buffer is declared and register task
 * or register timer function has already been used.
 */
uint8_t Sched_initBuffer( Sched_Scheduler *scheduler, uint8_t buffer ) {
    //local data.
    uint8_t status = 0;
    uint8_t i = 0;

    //Verifying which buffer to initialice.
    switch ( buffer ) {
        case 1://Timer buffer.
             for ( i = 0;  i < scheduler->timers; i++ ) {
                scheduler->timerPtr[i].callbackPtr = NULL;
            }
            status = 1;       
        break;
        case 2://Task buffer. 
            for ( i = 0;  i < scheduler->tasks; i++ ) {
                scheduler->taskPtr[i].taskFunc = NULL;
            }
            status = 1;
        break;
        default: status = 0; break;
    }

    return status;
}

/**
 * @brief This function confirms the register of a task.
 *
 * This function registers (operation = 0) an ID task in an static arrray or verifies (operation == 1) 
 * if a given ID task is present in an static array in order to confirm that the ID task is registered or not.
 *
 * @param[in] operation Indicator of operation to do, 0 register ID or 1 verify ID.
 * @param[in] task Task ID.
 *
 * @retval status Status of the operation, if operation == 0 successfull 1 or not 0, if operation == 1 ID not register 0
 * or ID already registered 1.
 */
uint8_t Sched_confirmTask_register( uint8_t operation, uint8_t task ) {
    //local data
    static uint8_t registered_IDs [n_tasks_h];//array of registered task´s IDs.
    static uint8_t index = 0;
    uint8_t status = false;

    //Verifying operation to do
    switch ( operation ) {
        case 0://register ID 
            registered_IDs[index] = task;
            index++;
            status = true;
        break;
        case 1://verify ID
            //Verifying if the ID is already registered. 
            if ( task != registered_IDs[task - 1] ) {//ID not yet registered.
                status = false;
            }

            else {//ID already registered.
                status = true; 
            }          
        break;
        default: status = false; break; 
    }

    if ( index > n_tasks_h - 1 ) {
        index = 0;//reseting.
    } 
    
    return status;
}

/**
 * @brief This function registers a new single task.
 *
 * This function registers a new single task in the buffer of tasks (TCB), for achieving this it checks the periodicity value
 * and verifies that the task to register is new.
 *
 * Once the buffer is full the new tasks to be registered will overwrite the last tasks registered in the buffer.
 * 
 * @param[in] scheduler Ptr to control struct instance, it allows the register of the task in the buffer of tasks.
 * @param[in] initPtr Ptr to init function of the task, it allows the call of the initialization function for the task.
 * @param[in] taskPtr Ptr to function of the task, it allows the call of the task function.
 * @param[in] period Periodicity value for the execution of the task.
 *
 * @retval task_id ID of the registered task, operation success ID == n, otherwise 0.
 *
 * @note The periodicity value must be >= tick and a multiple of tick.
 * @note The task must be a new task, repeated tasks are not allowed!
 */
uint8_t Sched_registerTask( Sched_Scheduler *scheduler, void (*initPtr)(void), void (*taskPtr)(void), uint32_t period ) {
    //local data 
    static uint8_t index = 0;//index for registering tasks in the buffer.
    static uint8_t first_call = true;
    uint8_t task_id  = 0;
    uint8_t i = 0;
    uint8_t new_task = true;

    //Verifying if periodicity value is valid according to tick value. Period >= tick, period must be multiple of tick.
    if ( ( period >= scheduler->tick ) && ( period % scheduler->tick == 0 ) ) {//valid periodicity.
        //If it´s the first call of the function all task function adresses initialice to 0.
        if ( first_call ) {
            Sched_initBuffer( scheduler, 2 );
            first_call = false;
        } 

        //Verifying if the task to register is new. 
        for ( i = 0; i < scheduler->tasks; i++ ) {
            if ( taskPtr != scheduler->taskPtr[i].taskFunc ) {
            new_task = new_task && true;
            }
            
            else {
              new_task = new_task && false;
              task_id = 0;//task already registered.
              break;  
            } 
        }
        
        if ( new_task ) {
            //Registering task.
            scheduler->taskPtr[index].period = period;
            scheduler->taskPtr[index].elapsed = 0;
            scheduler->taskPtr[index].initFunc = initPtr;
            scheduler->taskPtr[index].taskFunc = taskPtr;
            Bfx_ClrBit_u32u8( &scheduler->tasks_startFlags, index );//Flag disabled.
            task_id = index + 1;
            Sched_confirmTask_register( 0, task_id );//Confirming task register.

            index++;//index update.

            if ( index > scheduler->tasks - 1 ) {
                index = 0;//reseting.
            }   
        }
    }

    else {
        task_id = 0; //invalid periodicity.
    }

    return task_id;
}

/**
 * @brief This function stops a task.
 *
 * This function stops a registered task from being executed by disabling its start flag.
 *
 * @param[in] scheduler Ptr to control struct instance, it allows the stop of the task.
 * @param[in] task Task ID of the task to be stopped.
 *
 * @retval status Status of the operation, successfull 1 or not 0. 
 *
 * @note The task ID must be valid and also the task must be registered.
 */
uint8_t Sched_stopTask( Sched_Scheduler *scheduler, uint8_t task ) {
    //local data
    uint8_t status = false; 

    //Verifying if the task ID is valid.
    if ( ( task != 0 ) && ( task <= scheduler->tasks ) ) {//valid ID
        //Verifying if the task ID has been registered.
        if ( Sched_confirmTask_register( 1, task ) ) {//task ID already registered.
            Bfx_ClrBit_u32u8( &scheduler->tasks_startFlags, task - 1 );
            status = true;
        }

        else {
            status = false;//Task ID is not registered.
        }
    }

    else {
        status = false;//invalid ID.
    }

    return status;
}

/**
 * @brief This function starts a task.
 *
 * This function starts the execution of a registered task by enabling its start flag.
 *
 * @param[in] scheduler Ptr to control struct instance, it allows the start of the task.
 * @param[in] task Task ID of the task to be started.
 *
 * @retval status Status of the operation, successfull 1 or not 0. 
 *
 * @note The task ID must be valid and also the task must be registered.
 */
uint8_t Sched_startTask( Sched_Scheduler *scheduler, uint8_t task ) {
    //local data
    uint8_t status = false;

    //Verifying if the task ID is valid.
    if ( ( task != 0 ) && ( task <= scheduler->tasks ) ) {//valid ID
        //Verifying if the task ID has been registered.
        if ( Sched_confirmTask_register( 1, task ) ) {//task ID already registered.
            Bfx_SetBit_u32u8( &scheduler->tasks_startFlags, task - 1 );
            status = true;
        }

        else {
            status = false;//Task ID is not registered.
        }
    }

    else {
        status = false;//invalid ID.
    }
    
    return status;
}

/**
 * @brief This function changes the periodicity of a task.
 *
 * This function changes the periodicity of a registered task by modifying the period value of the task.
 *
 * @param[in] scheduler Ptr to control struct instance, it allows the change of the period value.
 * @param[in] task Task ID of the task to be modified.
 * @param[in] period Periodicity value to be set.
 *
 * @retval status Status of the operation, successfull 1 or not 0. 
 *
 * @note The periodicity value must be >= tick and a multiple of tick.
 * @note The task ID must be valid and the task must be registered.
 */
uint8_t Sched_periodTask( Sched_Scheduler *scheduler, uint8_t task, uint32_t period ){
    //local data.
    uint8_t status = false;

    //Verifying if the task ID is valid.
    if ( ( task != 0 ) && ( task <= scheduler->tasks ) ) {//valid ID
        //Verifying if the task ID has been registered.
        if ( Sched_confirmTask_register( 1, task ) ) {//task ID already registered.
            //Verifying if periodicity value is valid according to tick value. Period >= tick, period must be multiple of tick.
            if ( ( period >= scheduler->tick ) && ( period % scheduler->tick == 0 ) ) {//valid periodicity.
                scheduler->taskPtr[task - 1].period = period;
                status = true;
            }

            else {
                status = false;//invalid periodicity.
            }
        }

        else {
            status = false;//Task ID is not registered.
        }
    }

    else {
        status = false;//invalid ID.
    }

    return status;

}

/**
 * @brief This function verifies if it´s going to be the first execution of a task.
 *
 * This function verifies if it´s the first time that a task registered in the task buffer is going to be executed,
 * in order to determine if the function init is going to be launch or not.
 * 
 * It achieves this by having an static array which saves the memory addresses of the task functions that have
 * been executed.
 *
 * @param[in] scheduler Ptr to control structure instance, it allows the obtainment of the task function memory address.
 * @param[in] task ID of the task to verify.
 *
 * @retval status If it´s the first time the task is going to be executed it returns true, else returns false.
 */
uint8_t Sched_verifyTask_firstexe( Sched_Scheduler *scheduler, uint8_t task ) {
    //local data
    static uint64_t tasks_func_add[n_tasks_c];//array of memory addresses of the tasks functions that have been executed.
    static uint8_t index = 0;
    uint8_t i  = 0;
    uint8_t status = true;

    //Verifying if task to execute is new
    for ( i = 0; i < index + 1; i++ ) {
        if ( (unsigned long)scheduler->taskPtr[task - 1].taskFunc != tasks_func_add[i] ) {
            status = status && true;//task has never been executed.
        }

        else {//task has already been executed.
        status = status && false;
        break;
        }
    }

    //registering task function address.
    if ( status ) {
        tasks_func_add[index] = (unsigned long)scheduler->taskPtr[task - 1].taskFunc;
        index++;
        if (index > n_tasks_c - 1) {
            index = n_tasks_c - 1;
        }
    }

    return status;
}

/**
 * @brief This function confirms the register of a timer.
 *
 * This function registers (operation = 0) an ID timer in an static arrray or verifies (operation == 1) 
 * if a given ID timer is present in an static array in order to confirm that the ID timer is registered or not.
 *
 * @param[in] operation Indicator of operation to do, 0 register ID or 1 verify ID.
 * @param[in] timer Timer ID.
 *
 * @retval status Status of the operation, if operation == 0 successfull 1 or not 0, if operation == 1 ID not register 0
 * or ID already registered 1.
 */
uint8_t Sched_confirmTimer_register( uint8_t operation, uint8_t timer ) {
    //local data
    static uint8_t registered_IDs [n_timers_h];//array of registered timers´s IDs.
    static uint8_t index = 0;
    uint8_t status = false;

    //Verifying operation to do
    switch ( operation ) {
        case 0://register ID 
            registered_IDs[index] = timer;
            index++;
            status = true;
        break;
        case 1://verify ID
            //Verifying if the ID is already registered. 
            if ( timer != registered_IDs[timer - 1] ) {//ID not yet registered.
                status = false;
            }

            else {//ID already registered.
                status = true; 
            }          
        break;
        default: status = false; break; //invalid operation 
    }

    if ( index > n_timers_h - 1 ) {
        index = 0;//reseting.
    } 
    
    return status;
}

/**
 * @brief This function registers a new single timer.
 *
 * This function registers a new single timer in the buffer of timers (TCB), for achieving this it checks the timeout value
 * and verifies that the timer to register is new.
 *
 * Once the buffer is full the new timers to be registered will overwrite the last timers registered in the buffer.
 * 
 * @param[in] scheduler Ptr to control struct instance, it allows the register of the timer in the buffer of timers.
 * @param[in] timeout Timeout value for the execution of the callback function of the timer.
 * @param[in] callbackPtr Ptr to callback function of the timer, it allows the call of the timer callback function.
 * 
 * @retval timer_id ID of the registered timer, operation success ID == n, otherwise 0.
 *
 * @note The timeout value must be >= tick and a multiple of tick.
 * @note The timer must be a new timer, repeated timers are not allowed!
 */
uint8_t Sched_registerTimer( Sched_Scheduler *scheduler, uint32_t timeout, void (*callbackPtr)(void) ) {
    //local data 
    static uint8_t index = 0;//index for registering timers in the buffer.
    static uint8_t first_call = true;
    uint8_t timer_id  = 0;
    uint8_t i = 0;
    uint8_t new_timer = true;

    //Verifying if timeout value is valid according to tick value. timeout >= tick, timeout must be multiple of tick.
    if ( ( timeout >= scheduler->tick ) && ( timeout % scheduler->tick == 0 ) ) {//valid timeout.
        //If it´s the first call of the function all timers callback function adresses initialice to 0.
        if ( first_call ) {
            Sched_initBuffer( scheduler, 1 );
            first_call = false;
        } 

        //Verifying if the timer to register is new. 
        for ( i = 0; i < scheduler->timers; i++ ) {
            if ( callbackPtr != scheduler->timerPtr[i].callbackPtr ) {
            new_timer = new_timer && true;
            }
            
            else {
              new_timer = new_timer && false;
              timer_id = 0;//timer already registered.
              break;  
            } 
        }
        
        if ( new_timer ) {
            //Registering timer.
            scheduler->timerPtr[index].timeout = timeout;
            scheduler->timerPtr[index].count = timeout;
            scheduler->timerPtr[index].callbackPtr = callbackPtr;
            Bfx_ClrBit_u32u8( &scheduler->timers_startFlags, index );//Flag disabled.
            timer_id = index + 1;
            Sched_confirmTimer_register( 0, timer_id );//Confirming timer register.

            index++;//index update.

            if ( index > scheduler->timers - 1 ) {
                index = 0;//reseting.
            }   
        }
    }

    else {
        timer_id = 0; //invalid timeout.
    }

    return timer_id; 
}

/**
 * @brief This function starts a timer.
 *
 * This function starts the execution of a registered timer by enabling its start flag.
 * It also restarts the count value in case the timer has reach 0.
 *
 * @param[in] scheduler Ptr to control struct instance, it allows the start of the timer.
 * @param[in] timer Timer ID of the timer to be started.
 *
 * @retval status Status of the operation, successfull 1 or not 0. 
 *
 * @note The timer ID must be valid and also the timer must be registered.
 */
uint8_t Sched_startTimer( Sched_Scheduler *scheduler, uint8_t timer ) {
    //local data
    uint8_t status = false;

    //Verifying if the timer ID is valid.
    if ( ( timer != 0 ) && ( timer <= scheduler->timers ) ) {//valid ID
        //Verifying if the timer ID has been registered.
        if ( Sched_confirmTimer_register( 1, timer ) ) {//timer ID already registered.
            Bfx_SetBit_u32u8( &scheduler->timers_startFlags, timer - 1 );

            //Verifying if timer has reach 0.
            if ( scheduler->timerPtr[timer - 1].count == 0 ) {
                scheduler->timerPtr[timer - 1].count = scheduler->timerPtr[timer - 1].timeout;//restarting timer.
            }

            status = true;
        }

        else {
            status = false;//Timer ID is not registered.
        }
    }

    else {
        status = false;//invalid ID.
    }

    return status;
}

/**
 * @brief This function stops a timer.
 *
 * This function stops a registered timer from being executed by disabling its start flag.
 *
 * @param[in] scheduler Ptr to control struct instance, it allows the stop of the timer.
 * @param[in] timer Timer ID of the timer to be stopped.
 *
 * @retval status Status of the operation, successfull 1 or not 0. 
 *
 * @note The timer ID must be valid and also the timer must be registered.
 */
uint8_t Sched_stopTimer( Sched_Scheduler *scheduler, uint8_t timer ) {
    //local data
    uint8_t status = false; 

    //Verifying if the timer ID is valid.
    if ( ( timer != 0 ) && ( timer <= scheduler->timers ) ) {//valid ID
        //Verifying if the timer ID has been registered.
        if ( Sched_confirmTimer_register( 1, timer ) ) {//timer ID already registered.
            Bfx_ClrBit_u32u8( &scheduler->timers_startFlags, timer - 1 );
            status = true;
        }

        else {
            status = false;//Timer ID is not registered.
        }
    }

    else {
        status = false;//invalid ID.
    }

    return status;
}

/**
 * @brief This function gets the count value of a timer.
 *
 * This function gets the actual counter value of a given timer.
 *
 * @param[in] scheduler Ptr to control struct instance, it allows the obtainment of the count value of the timer.
 * @param[in] timer Timer ID of the timer to obtain count value.
 *
 * @retval counter_value Actual count value of timer, if operation was a success counter_value = n, otherwise 0. 
 *
 * @note The timer ID must be valid and also the timer must be registered.
 */
uint32_t Sched_getTimer( Sched_Scheduler *scheduler, uint8_t timer ) {
    //local data
    uint32_t counter_value = 0;

    //Verifying if the timer ID is valid.
    if ( ( timer != 0 ) && ( timer <= scheduler->timers ) ) {//valid ID
        //Verifying if the timer ID has been registered.
        if ( Sched_confirmTimer_register( 1, timer ) ) {//timer ID already registered.
            counter_value = scheduler->timerPtr[timer - 1].count;
        }

        else {
            counter_value = false;//Timer ID is not registered.
        }
    }

    else {
        counter_value = false;//invalid ID.
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
 * @param[in] scheduler Ptr to control struct instance, it allows the change of the timeout value.
 * @param[in] timer Timer ID of the timer to be modified.
 * @param[in] timeout Timeout value to be set.
 *
 * @retval status Status of the operation, successfull 1 or not 0. 
 *
 * @note The timeout value must be >= tick and a multiple of tick.
 * @note The timer ID must be valid and the timer must be registered.
 */
uint8_t Sched_reloadTimer( Sched_Scheduler *scheduler, uint8_t timer, uint32_t timeout ) {
    //local data
    uint8_t status = false;

    //Verifying if the timer ID is valid.
    if ( ( timer != 0 ) && ( timer <= scheduler->timers ) ) {//valid ID
        //Verifying if the timer ID has been registered.
        if ( Sched_confirmTimer_register( 1, timer ) ) {//timer ID already registered.
            //Verifying if timeout value is valid according to tick value. timeout >= tick, timeout must be multiple of tick.
            if ( ( timeout >= scheduler->tick ) && ( timeout % scheduler->tick == 0 ) ) {//valid timeout.
                scheduler->timerPtr[timer - 1].timeout = timeout;
                scheduler->timerPtr[timer - 1].count = timeout;//reseting timer to new reference value.
                Sched_startTimer( scheduler, timer );
                status = true;
            }

            else {
                status = false;//Invalid timeout.
            }
        }

        else {
            status = false;//Timer ID is not registered.
        }
    }

    else {
        status = false;//invalid ID.
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
 * @param[in] scheduler Ptr to control structure instance, it allows the execution of the scheduler.
 *
 * @note At least 1 timer and 1 task needs to be register in order to the scheduler to work.
 * @note After a timer reaches 0 is important to restart, stop or reload that timer else it will still decrement --> risk of overflow.
 * @note It´s important that previous to using the scheduler the buffers for the timers and task must be correctly defined, specially their flags.
 */
void Sched_startScheduler( Sched_Scheduler *scheduler ) {
    //local data
    uint32_t tick_value = OsIf_MicrosToTicks( scheduler->tick * 1000, OSIF_COUNTER_SYSTEM );//Translating tick value from us to counter increments.
    uint32_t count_value = OsIf_GetCounter( OSIF_COUNTER_SYSTEM );//First counter value capture.
    uint32_t elapsed_time = 0;//Time follow up for tick (timer increments).
    uint8_t i = 0;
    uint8_t new_task = false;

    //Initializing time follow-up for each task.
    for ( i = 0; i < scheduler->tasks; i++ ) {
        scheduler->taskPtr[i].elapsed = 0;
    }

    //Initializing counter values of all timers --> reseting all timers to their timeouts.
    for ( i = 0; i <scheduler->timers; i++ ) {
        scheduler->timerPtr[i].count = scheduler->timerPtr[i].timeout;
    }

    //Verifying which tasks registered are new.
    for ( i = 0; i < scheduler->tasks; i++ ) {
        if ( Bfx_GetBit_u32u8_u8( scheduler->tasks_startFlags, i ) ) {
            //Verifying if task is a new task.
            new_task = Sched_verifyTask_firstexe( scheduler, i + 1 );
            if ( new_task ) {
                scheduler->taskPtr[i].initFunc();//Executing init function.
            }
        }
    }

    while( 1 ) {//tasks are executed during timeout.
        //Time follow up update for tick.
        elapsed_time += OsIf_GetElapsed( &count_value, OSIF_COUNTER_SYSTEM );//time capture update
        
        //Verifying if a scheduler´s tick has occurred.
        if ( elapsed_time >= tick_value ) {          
            //Executing timers
            for ( i = 0; i < scheduler->timers; i++ ) {
                //Verifying execution of timer i
                if ( Bfx_GetBit_u32u8_u8( scheduler->timers_startFlags, i ) ) {//timer activated
                    scheduler->timerPtr[i].count -= scheduler->tick;//decrement
                    //Verifying execution of callback i
                    if ( Sched_getTimer(scheduler, i + 1) == 0 ) {
                        scheduler->timerPtr[i].callbackPtr();
                    }
                }
            }

            //Executing tasks.
            for ( i = 0; i < scheduler->tasks; i++ ) {
                //Time follow-up update for task i.
                scheduler->taskPtr[i].elapsed += scheduler->tick;
                
                //Verifying execution of task i
                if ( ( scheduler->taskPtr[i].elapsed >= scheduler->taskPtr[i].period ) && ( Bfx_GetBit_u32u8_u8( scheduler->tasks_startFlags, i ) ) ) {//periodicity achieved.
                    //Executing task function.
                    scheduler->taskPtr[i].taskFunc();

                    //Time follow-up reset for task i.
                    scheduler->taskPtr[i].elapsed = 0;                
                }  
            }

            elapsed_time = 0;//Reset time follow up for tick.
        }
    }
}

