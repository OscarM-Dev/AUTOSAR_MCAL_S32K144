/**
 * @file queue.h
 * @brief This header contains the declaration of the control structure and the functions 
 * related to the operations of the queue.
 */

#ifndef QUEUE_H
#define QUEUE_H
//Headers
#include <string.h>
#include <stdint.h>

//Control structure.
/**
  * @brief Control structure for operating the queue which members are 
  * related to the buffer and indexes for writing and reading.
*/
typedef struct {
    //members.
    void *Buffer;///< Ptr to array (Queue).
    uint32_t Elements;///< Number of elements of the queue.
    uint8_t Size;///< Number of bytes of each element of the queue.
    uint8_t Head;///< Index for writing.
    uint8_t Tail;///< Index for reading.
    uint8_t Empty;///< Bool flag that indicates if queue is empty.
    uint8_t Full;///< Bool flag that indicates if queue is full.
} Que_Queue;

//Operation functions.
/// @cond IGNORE
void Queue_initQueue( Que_Queue *queue );
uint8_t Queue_writeData( Que_Queue *queue, void *data );
uint8_t Queue_readData( Que_Queue *queue, void *data );
uint8_t Queue_isQueueEmpty( Que_Queue *queue );
void Queue_flushQueue( Que_Queue *queue );
/// @endcond

#endif