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
#include "Adc.h"
#include "HwIoAb_0_Buttons.h"
#include "HwIoAb_1_Leds.h"
#include "HwIoAb_2_Pots.h"


//Function declarations.
void MCU_clocks( void );
void EcuM_Init( void );
void HwIoAb_Init( void );



#endif