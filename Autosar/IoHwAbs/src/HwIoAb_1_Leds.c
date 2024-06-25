/**
 * @file HwIoAb_1_Leds.c
 * @brief This file contains the definition of the functions related to 
 * the IO hardware abstraction for the leds on board.
 * 
 */

//Headers.
#include "HwIoAb_1_Leds.h"

/**
 * @brief This function initialices the control structure for the leds.
 * 
 * @param [in] Leds_Config Pointer to configuration array.
 */
void HwIoAb_Leds_Init( const HwIoAb_Leds_Config *Leds_Config ) {
    //Initializing members.
    LedsControl_Ptr->Leds = HWIOAB_LEDS;
    LedsControl_Ptr->LedsConfig_Ptr = Leds_Config;
}

/**
 * @brief This function turns on a given led.
 * 
 * @param Led Led ID.
 * 
 * @note The led ID must be valid.
 */
void HwIoAb_Leds_TurnOn( uint8 Led ) {
    //local data,
    boolean active = FALSE;
    
    //Verifying if the ID is valid.
    if ( Led <= LedsControl_Ptr->Leds - 1 ) {   //Valid ID.
        //Defining active value according to active status of given led.
        if ( LedsControl_Ptr->LedsConfig_Ptr[ Led ].Active == HWIOAB_LED_ACTIVE_HIGH ) {
            active = STD_HIGH;
        }

        else {
            active = STD_LOW;
        }
        
        Dio_WriteChannel( LedsControl_Ptr->LedsConfig_Ptr[ Led ].Led, active );
    }
}

/**
 * @brief This function turns off a given led.
 * 
 * @param Led Led ID.
 * 
 * @note The led ID must be valid.
 */
void HwIoAb_Leds_TurnOff( uint8 Led ) {
    //local data,
    boolean active = FALSE;
    
    //Verifying if the ID is valid.
    if ( Led <= LedsControl_Ptr->Leds - 1 ) {   //Valid ID.
        //Defining active value according to active status of given led.
        if ( LedsControl_Ptr->LedsConfig_Ptr[ Led ].Active == HWIOAB_LED_ACTIVE_HIGH ) {
            active = STD_HIGH;
        }

        else {
            active = STD_LOW;
        }
        
        Dio_WriteChannel( LedsControl_Ptr->LedsConfig_Ptr[ Led ].Led, !active );
    }
}

/**
 * @brief This function toggles a given led.
 * 
 * @param Led Led ID.
 * 
 * @note The led ID must be valid.
 */
void HwIoAb_Leds_TurnToggle( uint8 Led ) {
    //Verifying if the ID is valid.
    if ( Led <= LedsControl_Ptr->Leds - 1 ) {   //Valid ID.
        Dio_FlipChannel( LedsControl_Ptr->LedsConfig_Ptr[ Led ].Led );
    }
}
