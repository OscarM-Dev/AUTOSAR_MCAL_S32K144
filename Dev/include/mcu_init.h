/**
 * @file Mcu_init.h
 * @brief This header contains the declaration of the initialization functions of the MCU.
 * 
 */

#ifndef MCU_INIT_H
#define MCU_INIT_H

//Headers.
#include "Mcu.h"
#include "OsIf.h"
#include "Platform.h"
#include "Port.h"
#include "IoHwAb.h"
#include "SEGGER_RTT.h"

//Function declarations.
void MCU_clocks( void );
void EcuM_Init( void );



#endif