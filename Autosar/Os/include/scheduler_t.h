/**
 * @file scheduler_t.h
 * @brief This header contains the declaration of the control structure and the functions of the round robin scheduler 
 * as well as the structure TCB for the tasks and timers.
 */

#ifndef SCHEDULER_T_H
#define SCHEDULER_T_H
//Headers.
#include <stdint.h>
#include "bool.h"
#include "OsIf.h"
#include "Bfx.h"


//Configuration parameters.
/**
  * @brief Configuration macros which indicate
  * the number of tasks and timers created and to handle, the last one being in charge of the size
  * of the queue.
  * 
  * @note The max number of tasks and timers to handle is 32.
*/
typedef enum { n_tasks_c = 32, n_tasks_h = 2, n_timers_c = 32, n_timers_h = 2 } config;

/** 
  * @defgroup Parameters, macros for operating the scheduler.
  @{ */
#define TICK 10 ///< Value for the time monitoring interval (ms)
/**
  @} */

//Control structures
/**
  * @brief Task control block (TCB) for the tasks to be managed which members are related
  * to the execution of each registered task.
*/
typedef struct _task
{   
    //members
    uint32_t period;          ///< Periodicity of the task.
    uint32_t elapsed;         ///< Time since last task execution (time follow up of task).
    void (*initFunc)(void);   ///< Pointer to init task function.
    void (*taskFunc)(void);   ///< Pointer to task function.
} Sched_Task;

/**
  * @brief Timer control block (TCB) for the timers to be managed which members are related
  * to the execution of each registered timer.
*/
typedef struct _Timer
{
    //members
    uint32_t timeout;       ///< Reference value for timer -->desired timing.
    uint32_t count;         ///< Timer counter value.
    void(*callbackPtr)(void); ///< Pointer to callback function.
} Sched_Timer;

/**
  * @brief Control structure to operate scheduler which members are related
  * to the time base and the buffers of tasks and timers.
*/
typedef struct _scheduler
{
    //members
    uint8_t tasksCount;    ///< Number of tasks created.
    uint8_t tasks;         ///< Number of tasks to handle/register in buffer.
    uint32_t tick;         ///< Time monitoring interval.
    Sched_Task *taskPtr;  ///< Pointer to buffer of tasks.
    uint32_t tasks_startFlags; ///< Start flags for tasks, each bit is a flag, bit n flag n.
    uint8_t timersCount; ///< Number of timers created.
    uint8_t timers;        ///< Number of timers to handle/register in buffer.
    Sched_Timer *timerPtr; ///< Pointer to buffer of timers.
    uint32_t timers_startFlags; ///< Start flags for timers, each bit is a flag, bit n flag n.
} Sched_Scheduler;

//Functions.
/// @cond IGNORE
uint8_t Sched_initScheduler( Sched_Scheduler *scheduler, uint8_t n_tasks_c, uint8_t n_tasks_h, uint32_t tick, Sched_Task *buffer_ta, uint8_t n_timers_c, uint8_t n_timers_h, Sched_Timer *buffer_ti );
uint8_t Sched_initBuffer( Sched_Scheduler *scheduler, uint8_t buffer );
uint8_t Sched_confirmTask_register( uint8_t operation, uint8_t task );
uint8_t Sched_registerTask( Sched_Scheduler *scheduler, void (*initPtr)(void), void (*taskPtr)(void), uint32_t period );
uint8_t Sched_verifyTask_firstexe( Sched_Scheduler *scheduler, uint8_t task );
uint8_t Sched_stopTask( Sched_Scheduler *scheduler, uint8_t task );
uint8_t Sched_startTask( Sched_Scheduler *scheduler, uint8_t task );
uint8_t Sched_periodTask( Sched_Scheduler *scheduler, uint8_t task, uint32_t period );
uint8_t Sched_registerTimer( Sched_Scheduler *scheduler, uint32_t timeout, void (*callbackPtr)(void) );
uint8_t Sched_confirmTimer_register( uint8_t operation, uint8_t timer );
uint8_t Sched_startTimer( Sched_Scheduler *scheduler, uint8_t timer );
uint8_t Sched_stopTimer( Sched_Scheduler *scheduler, uint8_t timer );
uint32_t Sched_getTimer( Sched_Scheduler *scheduler, uint8_t timer );
uint8_t Sched_reloadTimer( Sched_Scheduler *scheduler, uint8_t timer, uint32_t timeout );
void Sched_startScheduler( Sched_Scheduler *scheduler );
/// @endcond

#endif