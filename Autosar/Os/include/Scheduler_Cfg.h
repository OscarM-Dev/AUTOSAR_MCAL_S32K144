/**
 * @file Scheduler_Cfg.h
 * @brief This header contains the configuration macros of the round robin scheduler.
 * 
 */

#ifndef SCHEDULER_CFG_H
#define SCHEDULER_CFG_H

/** 
  * @defgroup Parameters, macros for operating the scheduler.
  @{ */
#define SCHEDULER_TICK 10 ///< Value for the time monitoring interval (ms).

#define SCHEDULER_MAX_TASKS 32  ///< Max number of tasks to handle.
#define SCHEDULER_MAX_TIMERS 32  ///< Max number of timers to handle.

#define SCHEDULER_TASKS 2  ///< Number of tasks to handle.
#define SCHEDULER_TIMERS 1   ///< Number of timers to handle.

#define SCHEDULER_TASK1_ID 1    ///< Task 1 Id.
#define SCHEDULER_TASK2_ID 2    ///< Task 2 Id.

#define SCHEDULER_TIMER1_ID 1   ///< Timer 1 Id.

#define SCHEDULER_TASK1_PERIOD_50MS 50    ///< Period value for task 1 in ms.
#define SCHEDULER_TASK2_PERIOD_100MS 100    ///< Period value for taks 2 in ms. 

#define SCHEDULER_TIMER1_TIMEOUT_300MS 300  ///< Timeout value for timer 1 in ms.

#define SCHEDULER_MAX_QUEUES 32 ///< Max number of queues to handle.

#define SCHEDULER_QUEUES 2  ///< Number of queues to handle.

#define SCHEDULER_QUEUE1_SIZE     sizeof( Queue1[0] ) ///< Number of bytes of each element of the queue 1.
#define SCHEDULER_QUEUE2_SIZE     sizeof( Queue2[0] ) ///< Number of bytes of each element of the queue 2. 

#define SCHEDULER_QUEUE1_ELEMENTS   3  ///< Number of elements of the queue 1.
#define SCHEDULER_QUEUE2_ELEMENTS   3  ///< Number of elements of the queue 2.

#define SCHEDULER_QUEUE_EMPTY_STATUS 1  ///< Empty status ID.
#define SCHEDULER_QUEUE_FULL_STATUS 2 ///< Full status ID.

/**
  @} */


#endif 