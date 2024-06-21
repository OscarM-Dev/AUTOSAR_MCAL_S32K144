/**
 * @file state_machine.c
 * @brief This file contains the definition of the functions related to the double click state machine.
 * 
 * This operation is based on the flow chart diagram.
 * 
 * @note This function is called every 50ms.
 * @note Remember that the 300ms count is for detecting a hold click when the button is pressed 1 and 2 times.
 * @note A single click toggles PTC8.
 * @note A double click toggles PTC9.
 * @note A hold click toggles PTC10.
 * 
 */

//Headers
#include "state_machine.h"

/**
 * @brief This function is the double click state machine.
 * 
 */
void DoubleClick_stMachine( void ) {
   /* switch( state ) {
        case IDLE :
            //Checks if the button is pressed.
            if ( Dio_ReadChannel( DioConf_DioChannel_PTE12 ) == STD_LOW ) {
                //Gpt_StartTimer( GptConf_GptChannelConfiguration_Gpt_Ftm0_ch1, FTM0CH1_V );//Starting timer 300ms.
                state = SINGLE_PRESS;
            }
        break;
        
        case SINGLE_PRESS :
            //Checking timer timeout.
            if ( Gpt_GetTimeRemaining( GptConf_GptChannelConfiguration_Gpt_Ftm0_ch1 ) == 0 ) {
                state = HOLD;
            }

            //Checking if the button is released.
            if ( Dio_ReadChannel( DioConf_DioChannel_PTE12 ) == STD_HIGH ) {
                state = SINGLE_RELEASE;
            }
        break;
        
        case SINGLE_RELEASE :
            //Checking timer timeout.
            if ( Gpt_GetTimeRemaining( GptConf_GptChannelConfiguration_Gpt_Ftm0_ch1 ) == 0 ) {//Single click
                Dio_FlipChannel( DioConf_DioChannel_PTC8 );
                state = IDLE;
            }

            //Checks if the button is pressed.
            if ( Dio_ReadChannel( DioConf_DioChannel_PTE12 ) == STD_LOW ) {
                Gpt_StartTimer( GptConf_GptChannelConfiguration_Gpt_Ftm0_ch1, FTM0CH1_V );//Starting timer 300ms.
                state = DOUBLE_PRESS;
            }
        break;
        
        case DOUBLE_PRESS :
            //Checking timer timeout.
            if ( Gpt_GetTimeRemaining( GptConf_GptChannelConfiguration_Gpt_Ftm0_ch1 ) == 0 ) {
                state = HOLD;
            }

            //Checking if the button is released.
            if ( Dio_ReadChannel( DioConf_DioChannel_PTE12 ) == STD_HIGH ) {//Double click.
                Dio_FlipChannel( DioConf_DioChannel_PTC9 );
                state = IDLE;
            }
        break;
        
        case HOLD ://Hold click.
            //Checking if the button is released.
            if ( Dio_ReadChannel( DioConf_DioChannel_PTE12 ) == STD_HIGH ) {
                Dio_FlipChannel( DioConf_DioChannel_PTC10 );
                state = IDLE;
            }
        break;
        
        default :
        break;
    } */
}
