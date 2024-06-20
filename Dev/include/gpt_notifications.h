/**
 * @file gpt_notifications.h
 * @brief This header contains the declaration of the notifications of the gpt driver.
 *
 * 
 */

#ifndef GPT_NOTIFICATIONS_H
#define GPT_NOTIFICATIONS_H

//Headers
#include "Platform.h"
#include "Gpt.h"
#include "Dio.h"
#include "state_machine.h"

//Functions
void Ftm0_ch0_callback( void );
void Ftm0_ch1_callback( void );

#endif
