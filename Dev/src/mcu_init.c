/**
 * @file mcu_init.c
 * @brief This file contains the definition of the initialization functions of the MCU.
 * 
 */

//Headers
#include "Mcu_init.h"

/**
 * @brief This function configures the clock signals of the MCU.
 * 
 * This function configures the next clock signals:
 *  -Core_clk --> 48Mhz
 *  -Sys_clk --> 48Mhz
 *  -Bus_clk --> 48Mhz
 *  -Flash_clk --> 24Mhz
 * 
 *  -SOCS_clk -->Disabled
 *  -PLL_CLK -->Disabled
 * 
 *  -SPLLDIV1_CLK -->Disabled
 *  -SPLLDIV2_CLK -->Disabled
 *  -FIRCDIV1_CLK -->Disabled
 *  -FIRCDIV2_CLK -->Disabled
 *  -SIRCDIV1_CLK -->Disabled
 *  -SIRCDIV2_CLK -->8 Mhz
 *  -SOSCDIV1_CLK  -->Disabled
 *  -SOSCDIV2_CLK  -->Disabled
 */
void MCU_clocks( void ) {
    /*Init Mcu module, including internal PLL, reference to Mcu Config structure can 
    be found at Mcu_PBcfg.h and PLL defines at Mcu_Cfg.h*/
    Mcu_Init( &Mcu_Config );//Initializing MCU driver
    Mcu_SetMode( Run_mode );//MCU on run mode.
    Mcu_InitClock( Clocks_conf_0 );//Executing clock configurations established in container.

    //Configuring Pll clock.
    /*while ( Mcu_GetPllStatus() != MCU_PLL_LOCKED );//Waiting until PLL is locked.
    Mcu_DistributePllClock();//Executing pll clock configuration established in container.*/
}

/**
 * @brief This function initialices the microcontroller and the peripherals
 * 
 * 
 */
void EcuM_Init( void )
{   
    MCU_clocks();
    OsIf_Init( NULL_PTR );  //Init Osif timer.
    Platform_Init( NULL_PTR );  //Init platform driver and configuration.
    Port_Init( &Port_Config );  //Init port driver and configuration.
    Adc_Init( &Adc_Config );    //Init Adc driver and configuration.
    
    //Configuring interleave for PTB1 and PTB13.
    Port_Ci_Port_Ip_SetMuxModeSel( IP_PORTB, 1, PORT_MUX_ADC_INTERLEAVE );
    Port_Ci_Port_Ip_SetMuxModeSel( IP_PORTB, 13, PORT_MUX_ADC_INTERLEAVE );
}

/**
 * @brief This function initialices the hardware io abstractions.
 * 
 */
void HwIoAb_Init( void ) {
    HwIoAb_Buttons_Init( ButtonsCfg );  //Buttons init.
    HwIoAb_Leds_Init( LedsCfg );    //Leds init.
    HwIoAb_Pots_Init( NULL_PTR );   //Pots init.
}