/**
 * @file Scheduler.h
 * @brief This header contains the declaration of the control structure and the functions of the round robin scheduler 
 * as well as the structure TCB for the tasks and timers.
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H
//Headers.
#include "StandardTypes.h"
#include "Scheduler_Cfg.h"
#include "OsIf.h"
#include "Bfx.h"

//Types
typedef uint32 TickType; ///< Type for tick value.
typedef uint8 TaskType; ///< Type for the number of tasks to handle.
typedef uint8 TimerType; ///< Type for the number of timers to handle.

//Control structures
/**
  * @brief Task control block (TCB) for the tasks to be managed which members are related
  * to the execution of each registered task.
  * @note Usually the instances of this struct is constant.
*/
typedef struct _Task_ConfigType {   
    //members
    uint32 InitPeriod;          ///< Initial periodicity of the task in ms.
    uint8 InitFlag;             ///< Initial flag value.
    void ( *TaskFunc ) ( void );   ///< Pointer to task function.
} Task_ConfigType;

/**
  * @brief Timer control block (TCB) for the timers to be managed which members are related
  * to the execution of each registered timer.
  * @note Usually the instances of this struct is constant.
*/
typedef struct _Timer_ConfigType {
    //members
    uint32 InitTimeout;       ///< Initial reference value for timer -->desired timing in ms.
    uint8 InitFlag;           ///< Initial flag value.
    void ( *CallbackFunc ) ( void ); ///< Pointer to callback function.
} Timer_ConfigType;

/**
  * @brief Control structure 1 to operate scheduler which members are related
  * to the time base and the buffers of tasks and timers.
  * @note Usually the instance of this struct is constant.
*/
typedef struct _Scheduler_ConfigType {
    //members
    TickType Tick;         ///< Time monitoring interval in ms.
    TaskType Tasks;        ///< Number of tasks to handle/register in buffer.
    const Task_ConfigType *TaskPtr;  ///< Pointer to buffer of tasks.
    TimerType Timers;       ///< Number of timers to handle/register in buffer.
    const Timer_ConfigType *TimerPtr; ///< Pointer to buffer of timers.
} Scheduler_ConfigType;

/**
  * @brief Control structure 2 to operate scheduler which members are related
  * to the time base and the buffers of tasks and timers.
  * @note Usually the instance of this struct is variable.
*/
typedef struct _Scheduler_CtrlType { 
    //members
    uint32 TaskPeriod[ SCHEDULER_TASKS ]; ///< Actual periodicity values of the tasks in ms, element n is the periodicity of task n (id - 1).
    uint32 TaskElapsed[ SCHEDULER_TASKS ]; ///< Time since last task execution (time follow up of task), element n is the time follow up of task n (id - 1).
    uint32 TimerTimeout[ SCHEDULER_TIMERS ];  ///< Actual reference values for timers -->desired timing in ms, element n is the reference value for timer n (id - 1).
    uint32 TimerCount[ SCHEDULER_TIMERS ]; ///< Timers counter values, element n is the counter value of timer n (id - 1).
    uint32 TaskFlags; ///< Start flags for tasks, each bit is a flag, bit n flag n.
    uint32 TimerFlags; ///< Start flags for timers, each bit is a flag, bit n flag n.
} Scheduler_CtrlType;

//Reference to global data.
extern const Scheduler_ConfigType Scheduler_Config; //Control structure 1 instance.
extern Scheduler_CtrlType Scheduler_Control; //Control structure 2 instance.
extern const Scheduler_ConfigType *SchedulerConfig_Ptr;//Pointer to control structure 1.
extern Scheduler_CtrlType *SchedulerCtrl_Ptr; //Pointer to control structure 2.

//Functions.
/// @cond IGNORE
Std_ReturnType Scheduler_Init( Scheduler_CtrlType *SchedulerPtr );
Std_ReturnType Scheduler_StopTask( TaskType Task );
Std_ReturnType Scheduler_StartTask( TaskType Task );
Std_ReturnType Scheduler_PeriodTask( TaskType Task, uint32 NewPeriod );
Std_ReturnType Scheduler_StartTimer( TimerType Timer );
Std_ReturnType Scheduler_StopTimer( TimerType Timer );
uint32 Scheduler_GetTimer( TimerType Timer );
Std_ReturnType Scheduler_ReloadTimer( TimerType Timer, uint32 NewTimeout );
void Scheduler_MainFunction( void );

//Reference to callback functions.
extern void Task1_callback( void );
extern void Task2_callback( void );
extern void Timer1_callback( void );
extern void Timer2_callback( void );

/// @endcond

#endif