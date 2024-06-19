/**
 * @file queue.c
 * @brief This file contains the operation functions definitions of the queue.
 *
 * The queue is basically a circular buffer that can handle any kind of data, this file has the functions 
 * related to it´s operation.
 */

//Header
#include "queue.h"

/**
 * @brief This function initialices the queue.
 *
 * This function initialices the queue in order to be used 
 * by initializing the members of the control structure.
 *
 * @param[in] queue Ptr to control structure instance, allows the init of the members.
 */
void Queue_initQueue( Que_Queue *queue ) {
    queue->Empty = 1;
    queue->Full = 0;
    queue->Head = 0;
    queue->Tail = 0;
}

/**
 * @brief This function indicates if the queue is empty.
 *
 * This function verifies the empty bool flag of the queue control structure.
 *
 * @param[in] queue Ptr to control structure instance, allows the access to the empty bool flag.
 *
 * @retval Empty Bool flag 1 (queue empty) or 0 (queue not empty).
*/
uint8_t Queue_isQueueEmpty( Que_Queue *queue ){
    return queue->Empty;    
}

/**
 * @brief This function empties the queue.
 *
 * This function empties the queue in case it has data available if not it remains the same.
 * It achieves this by calling the init queue function.
 *
 * @param[in] queue Ptr to control structure instance, this parameter is passed to the init queue function.
*/
void Queue_flushQueue( Que_Queue *queue ) {
    Queue_initQueue( queue );//Restarting queue.
}

/**
 * @brief This function writes a single data to the queue.
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
 * @param[in] queue Ptr to control structure instance, allows the access to the head element of the queue.
 * @param[in] data Void ptr to the data to be written, allows the access to the data.
 *
 * @retval status Status of the write operation, write operation was succesfull 1 or not 0.
*/
uint8_t Queue_writeData( Que_Queue *queue, void *data ){
    //Local data
    uint64_t base_add = (unsigned long long) queue->Buffer;//queue element 0 memory address.
    uint64_t actual_add = 0;//memory address of the actual queue element.
    uint8_t status = 0;

    //Verifying if the queue is availabe to be written.
    if ( !queue->Full ){//Spaces available.

        //Writing to array, copying and pasting size bytes of info from data to queue.
        actual_add = base_add + ( queue->Head * queue->Size );
        memcpy( (void *)actual_add, data, queue->Size );

        queue->Head++;//next value to be written.

        if ( queue->Head > queue->Elements - 1 ) queue->Head = 0;//Reseting write index.

        if ( queue->Empty ) queue->Empty = !queue->Empty;//Once data is written the queue is no longer empty.

        //Verifying if queue is full with the last data written.
        if ( queue->Head == queue->Tail ) queue->Full = 1;

        status = 1;//Succesfull operation.
    }
    return status; 
}

/**
 * @brief This function reads a single data from the queue.
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
 * @param[in] queue Ptr to control structure instance, allows the access to the tail element of the queue.
 * @param[in] data Void ptr to the variable to put the read data from the queue, allows the read from the queue.
 *
 * @retval status Status of the read operation, read operation was succesfull 1 or not 0.
*/
uint8_t Queue_readData( Que_Queue*queue, void *data ){
    //local data
    uint64_t base_add = (unsigned long long) queue->Buffer;//queue element 0 memory address.
    uint64_t actual_add = 0;//memory address of the actual queue element.
    uint8_t status = 0;

    //Verifying if the queue has data available to be read.
    if (!queue->Empty) { 
        
        //Reading from array, copying and pasting size bytes of info from queue to data.
        actual_add = base_add + ( queue->Tail * queue->Size );
        memcpy( data, (void*)actual_add, queue->Size );
        
        queue->Tail++;//next value to be read.

        if ( queue->Tail > queue->Elements - 1 ) queue->Tail = 0;//Reseting read index.

        if ( queue->Full ) queue->Full = !queue->Full;//Once data is read the queue is no longer full.

        //Verifying if the queue is empty with the last data read.
        if ( queue->Tail == queue->Head ) queue->Empty = 1;
        
        status = 1;//Succesfull operation.            
    }
    return status;
}