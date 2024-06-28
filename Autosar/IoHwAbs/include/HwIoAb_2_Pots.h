/**
 * @file HwIoAb_2_Pots.h
 * @brief This header contains the declaration of the control structures, functions and global data related to 
 * the IO hardware abstraction for the pots on board.
 */

#ifndef HWIOAB_2_POTS_H
#define HWIOAB_2_POTS_H

//Headers.
#include "StandardTypes.h"
#include "Adc.h"

#include "HwIoAb_2_Pots_Cfg.h"

//Config structures.
/**
 * @brief Configuration structure for each Pot
 * 
 * @note This IO abstraction is no configurable.
 * 
 */
typedef struct _HwIoAb_Pots_Config {
    uint8 dummy;
} HwIoAb_Pots_Config;

//Control structures.
/**
 * @brief Control/Config structure for the pots on board.
 * 
 * @note Usually the instance of this struct is variable.
 * @note Each element of the arrays is the conversion result of a pot.
 * @note Element n is pot n.
 * @note In this case we have only 2 ADC groups, one for AC0 and the other for ADC1.
 * @note All pots have the same total resistance value.
 * 
 */
typedef struct _HwIoAb_Pots_Ctrl {
    uint8 Pots;  ///< Number of pots to handle.
    uint16 Raw_results_main[ HWIOAB_POTS ];     ///< Raw conversion results array for the main ADC (0).
    uint16 Raw_results_alter[ HWIOAB_POTS ];    ///< Raw conversion results array for the alter ADC (1).
    uint16 Ohms_results_main[ HWIOAB_POTS ];    ///< Ohms conversion results array for the main ADC (0).
    uint16 Ohms_results_alter[ HWIOAB_POTS ];   ///< Ohms conversion results array for the main ADC (1).
} HwIoAb_Pots_Ctrl;

//Reference to global data.
extern HwIoAb_Pots_Ctrl PotsCtrl;
extern HwIoAb_Pots_Ctrl *PotsControl_Ptr;

//Functions.
void HwIoAb_Pots_Init( const HwIoAb_Pots_Config *Pots_Config );
void HwIoAb_Pots_GetValue( uint16 *Pots );
void HwIoAb_Pots_GetAltValue( uint16 *AltPots );

#endif