/*==================================================================================================
*   Project              : RTD AUTOSAR 4.7
*   Platform             : CORTEXM
*   Peripheral           : 
*   Dependencies         : none
*
*   Autosar Version      : 4.7.0
*   Autosar Revision     : ASR_REL_4_7_REV_0000
*   Autosar Conf.Variant :
*   SW Version           : 2.0.0
*   Build Version        : S32K1_RTD_2_0_0_D2308_ASR_REL_4_7_REV_0000_20230804
*
*   (c) Copyright 2020-2023 NXP Semiconductors
*   All Rights Reserved.
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/

/**
*   @file Det.c
*
*   @addtogroup DET_MODULE
*   @{
*/


#ifdef __cplusplus
extern "C"{
#endif

/*
* @page misra_violations MISRA-C:2012 violations
*
* @section Det_c_REF_1
* Violates MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents of a header file being included more than once.
* This violation is not fixed since the inclusion of <MA>_MemMap.h is as per AUTOSAR requirement [SWS_MemMap_00003].
*
* @section Det_c_REF_2
* Violates MISRA 2012 Advisory Rule 20.1, #Include directives should only be preceded by preprocessor directives or comments.
* <MA>_MemMap.h is included after each section define in order to set the current memory section as defined by AUTOSAR.
*/

/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
/**
* @file           Det.c
* @requirements   DET004
*/
#include "Mcal.h"
#include "OsIf.h"
#include "Det.h"
#include "Mcu.h"
#include "Platform.h"
#include "Port.h"
#include "Dio.h"
#include "Pwm.h"
#include "Adc.h"
#include "SEGGER_RTT.h"

/*==================================================================================================
*                                        LOCAL MACROS
==================================================================================================*/
#define DET_VENDOR_ID_C                     43
#define DET_AR_RELEASE_MAJOR_VERSION_C      4
#define DET_AR_RELEASE_MINOR_VERSION_C      7
#define DET_AR_RELEASE_REVISION_VERSION_C   0
#define DET_SW_MAJOR_VERSION_C              2
#define DET_SW_MINOR_VERSION_C              0
#define DET_SW_PATCH_VERSION_C              0
/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/
/* Check if source file and DET header file are of the same vendor */
#if (DET_VENDOR_ID_C != DET_VENDOR_ID)
    #error "Det.c and Det.h have different vendor ids"
#endif

/* Check if source file and DET header file are of the same Autosar version */
#if ((DET_AR_RELEASE_MAJOR_VERSION_C != DET_AR_RELEASE_MAJOR_VERSION) || \
     (DET_AR_RELEASE_MINOR_VERSION_C != DET_AR_RELEASE_MINOR_VERSION) || \
     (DET_AR_RELEASE_REVISION_VERSION_C != DET_AR_RELEASE_REVISION_VERSION))
  #error "AutoSar Version Numbers of Det.c and Det.h are different"
#endif

/* Check if source file and DET header file are of the same Software version */
#if ((DET_SW_MAJOR_VERSION_C != DET_SW_MAJOR_VERSION) || \
     (DET_SW_MINOR_VERSION_C != DET_SW_MINOR_VERSION) || \
     (DET_SW_PATCH_VERSION_C != DET_SW_PATCH_VERSION))
    #error "Software Version Numbers of Det.c and Det.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if DET source file and STD header file are of the same Autosar version */
    #if ((DET_AR_RELEASE_MAJOR_VERSION_C != STD_AR_RELEASE_MAJOR_VERSION) || \
        (DET_AR_RELEASE_MINOR_VERSION_C != STD_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Det.c and Std_Types.h are different"
    #endif
#endif
/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                       GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                       GLOBAL VARIABLES
==================================================================================================*/
#define DET_START_SEC_VAR_CLEARED_8_NO_CACHEABLE
/*
 * @violates @ref Det_c_REF_1 Precautions shall be taken in order to prevent the contents of a header file being included more than once
 * @violates @ref Det_c_REF_2 #Include directives should only be preceded by preprocessor directives or comments
 */
#include "Det_MemMap.h"
/* Variables to store last DET error */
uint8 Det_InstanceId[DET_NO_ECU_CORES];                /**< @brief DET instance ID*/
uint8 Det_ApiId[DET_NO_ECU_CORES];                     /**< @brief DET API ID*/
uint8 Det_ErrorId[DET_NO_ECU_CORES];                   /**< @brief DET Error ID*/
/* Variables to store last DET runtime error */
uint8 Det_RuntimeInstanceId[DET_NO_ECU_CORES];         /**< @brief DET instance ID*/
uint8 Det_RuntimeApiId[DET_NO_ECU_CORES];              /**< @brief DET API ID*/
uint8 Det_RuntimeErrorId[DET_NO_ECU_CORES];            /**< @brief DET Error ID*/
/* Variables to store last DET transient error */
uint8 Det_TransientInstanceId[DET_NO_ECU_CORES];       /**< @brief DET instance ID*/
uint8 Det_TransientApiId[DET_NO_ECU_CORES];            /**< @brief DET API ID*/
uint8 Det_TransientFaultId[DET_NO_ECU_CORES];          /**< @brief DET Error ID*/
#define DET_STOP_SEC_VAR_CLEARED_8_NO_CACHEABLE

char *ErrorString_Ptr[3];   //Array of pointers to strings for the error message.

/*
 * @violates @ref Det_c_REF_1 Precautions shall be taken in order to prevent the contents of a header file being included more than once
 * @violates @ref Det_c_REF_2 #Include directives should only be preceded by preprocessor directives or comments
 */
#include "Det_MemMap.h"

#define DET_START_SEC_VAR_CLEARED_16_NO_CACHEABLE
/*
 * @violates @ref Det_c_REF_1 Precautions shall be taken in order to prevent the contents of a header file being included more than once
 * @violates @ref Det_c_REF_2 #Include directives should only be preceded by preprocessor directives or comments
 */
#include "Det_MemMap.h"
uint16 Det_TransientModuleId[DET_NO_ECU_CORES];       /**< @brief DET module ID*/
uint16 Det_ModuleId[DET_NO_ECU_CORES];       /**< @brief DET module ID*/
uint16 Det_RuntimeModuleId[DET_NO_ECU_CORES];       /**< @brief DET module ID*/
#define DET_STOP_SEC_VAR_CLEARED_16_NO_CACHEABLE
/*
 * @violates @ref Det_c_REF_1 Precautions shall be taken in order to prevent the contents of a header file being included more than once
 * @violates @ref Det_c_REF_2 #Include directives should only be preceded by preprocessor directives or comments
 */
#include "Det_MemMap.h"


/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
/* put all DET code into defined section */

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
#define DET_START_SEC_CODE
#include "Det_MemMap.h"

/*================================================================================================*/
/**
* @brief          This function initializes the DET module.
* @details        This is a function stub only. Functionality is void in this implementation
*
* @requirements   DET008
*
*/
void Det_Init(void)
{
    /* Do nothing */
}

/*================================================================================================*/
/**
 * @brief This function gets the string name corresponding to a module Id.
 * 
 * @param[in] ModuleId Module Id where error was detected.
 * @return StringPtr Pointer to string module name.
 */
char* Det_GetModuleName( uint16 ModuleId ) {
    switch ( ModuleId ) {
        case MCU_MODULE_ID: ErrorString_Ptr[0] = "Mcu module"; break;
        case CDD_PLATFORM_MODULE_ID: ErrorString_Ptr[0] = "Platform module"; break;
        case PORT_MODULE_ID: ErrorString_Ptr[0] = "Port module"; break;
        case DIO_MODULE_ID: ErrorString_Ptr[0] = "Dio module"; break;
        case PWM_MODULE_ID: ErrorString_Ptr[0] = "Pwm module"; break;
        case ADC_MODULE_ID: ErrorString_Ptr[0] = "Adc module"; break;

        default: break;    //Module ID is not used in aplication.
    }

    return ErrorString_Ptr[0];
}

/*================================================================================================*/
/**
 * @brief This function gets the string name corresponding to an Api Id.
 * 
 * @param[in] ModuleId Module Id where error was detected. 
 * @param[in] ApiId Api Id where error was detected 
 * @return StringPtr Pointer to string Api name.
 */
char* Det_GetApiName( uint16 ModuleId, uint8 ApiId ) {

    switch ( ModuleId ) {
        case MCU_MODULE_ID:
            switch( ApiId ) {   //4 Apis that report det errors.
                case MCU_INIT_ID: ErrorString_Ptr[1] = "Mcu_Init()"; break;
                case MCU_INITCLOCK_ID: ErrorString_Ptr[1] = "Mcu_InitClock()"; break;
                case MCU_SETMODE_ID: ErrorString_Ptr[1] = "Mcu_SetMode()"; break;
                case MCU_GETVERSIONINFO_ID: ErrorString_Ptr[1] = "Mcu_GetVersionInfo()"; break;
                default: break; //Api doesnt report det error or is not used.
            }
        break;

        case CDD_PLATFORM_MODULE_ID:
            switch ( ApiId ) {  //5 Apis that report det errors.
                case PLATFORM_INIT_ID: ErrorString_Ptr[1] = "Platform_Init()"; break;
                case PLATFORM_SET_IRQ_ID: ErrorString_Ptr[1] = "Platform_SetIrq()"; break;
                case PLATFORM_SET_IRQ_PRIO_ID: ErrorString_Ptr[1] = "Platform_SetIrqPriority()"; break;
                case PLATFORM_GET_IRQ_PRIO_ID: ErrorString_Ptr[1] = "Platform_GetIrqPriority()"; break;
                case PLATFORM_INSTALL_HANDLER_ID: ErrorString_Ptr[1] = "Platform_InstallIrqHandler()"; break;
                default: break; //Api doesnt report det error or is not used.
            }
        break;

        case PORT_MODULE_ID:
            switch ( ApiId ) {  //5 Apis that report det errors.
                case PORT_INIT_ID: ErrorString_Ptr[1] = "Port_Init()"; break;
                case PORT_SETPINDIRECTION_ID: ErrorString_Ptr[1] = "Port_SetPinDirection()"; break;
                case PORT_SETPINMODE_ID: ErrorString_Ptr[1] = "Port_SetPinMode()"; break;
                case PORT_REFRESHPINDIRECTION_ID: ErrorString_Ptr[1] = "Port_RefreshPortDirection()"; break;
                case PORT_GETVERSIONINFO_ID: ErrorString_Ptr[1] = "Port_GetVersionInfo()"; break;
                default: break; //Api doesnt report det error or is not used.
            }
        break;

        case DIO_MODULE_ID:
            switch ( ApiId ) {  //8 Apis that report det errors.
                case DIO_WRITECHANNEL_ID: ErrorString_Ptr[1] = "Dio_WriteChannel()"; break;
                case DIO_READCHANNEL_ID: ErrorString_Ptr[1] = "Dio_ReadChannel()"; break;
                case DIO_FLIPCHANNEL_ID: ErrorString_Ptr[1] = "Dio_FlipChannel()"; break;
                case DIO_WRITECHANNELGROUP_ID: ErrorString_Ptr[1] = "Dio_WriteChannelGroup()"; break;
                case DIO_READCHANNELGROUP_ID: ErrorString_Ptr[1] = "Dio_ReadChannelGroup()"; break;
                case DIO_READPORT_ID: ErrorString_Ptr[1] = "Dio_ReadPort()"; break;
                case DIO_WRITEPORT_ID: ErrorString_Ptr[1] = "Dio_WritePort()"; break;
                case DIO_GETVERSIONINFO_ID: ErrorString_Ptr[1] = "Dio_GetVersionInfo()"; break;
                default: break; //Api doesnt report det error or is not used.
            }
        break;

        case PWM_MODULE_ID:
            switch ( ApiId ) {  //6 Apis that report det errors.
                case PWM_INIT_ID: ErrorString_Ptr[1] = "Pwm_Init()"; break;
                case PWM_DEINIT_ID: ErrorString_Ptr[1] = "Pwm_DeInit()"; break;
                case PWM_SETDUTYCYCLE_ID: ErrorString_Ptr[1] = "Pwm_SetDutyCycle()"; break;
                case PWM_SETPERIODANDDUTY_ID: ErrorString_Ptr[1] = "Pwm_SetPeriodAndDuty()"; break;
                case PWM_SETOUTPUTTOIDLE_ID: ErrorString_Ptr[1] = "Pwm_SetOutputToIdle()"; break;
                case PWM_GETVERSIONINFO_ID: ErrorString_Ptr[1] = "Pwm_GetVersionInfo()"; break;
                default: break; //Api doesnt report det error or is not used.
            }
        break;

        case ADC_MODULE_ID:
            switch ( ApiId ) {  //9 Apis that report det errors.
                case ADC_INIT_ID: ErrorString_Ptr[1] = "Adc_Init()"; break;
                case ADC_DEINIT_ID: ErrorString_Ptr[1] = "Adc_DeInit()"; break;
                case ADC_STARTGROUPCONVERSION_ID: ErrorString_Ptr[1] = "Adc_StartGroupConversion()"; break;
                case ADC_STOPGROUPCONVERSION_ID: ErrorString_Ptr[1] = "Adc_StopGroupConversion()"; break;
                case ADC_VALUEREADGROUP_ID: ErrorString_Ptr[1] = "Adc_ReadGroup()"; break;
                case ADC_GETGROUPSTATUS_ID: ErrorString_Ptr[1] = "Adc_GetGroupStatus()"; break;
                case ADC_CALIBRATE_ID: ErrorString_Ptr[1] = "Adc_Calibrate()"; break;
                case ADC_SETUPRESULTBUFFER_ID: ErrorString_Ptr[1] = "Adc_SetupResultBuffer()"; break;
                case ADC_GETVERSIONINFO_ID: ErrorString_Ptr[1] = "Adc_GetVersionInfo()"; break;
                default: break; //Api doesnt report det error or is not used.
            }
        break;

        default: //Module ID is not used in aplication.
        break;
    }

    return ErrorString_Ptr[1];
}

/*================================================================================================*/
/**
 * @brief This function gets the error meaning according to the Error Id.
 * 
 * @param[in] ModuleId Module Id where error was detected.
 * @param[in] ErrorId Error Id reported. 
 * @return StringPtr Pointer to string Error meaning.
 */
char* Det_GetErrorMeaning( uint16 ModuleId, uint8 ErrorId ) {

    switch( ModuleId ) {
        case MCU_MODULE_ID: //6 errors.
            switch ( ErrorId ) {
                case MCU_E_INIT_FAILED: ErrorString_Ptr[2] = "Invalid conf pointer"; break;
                case MCU_E_PARAM_CLOCK: ErrorString_Ptr[2] = "Invalid input parameter"; break;
                case MCU_E_PARAM_MODE: ErrorString_Ptr[2] = "Invalid input parameter"; break;
                case MCU_E_PARAM_POINTER: ErrorString_Ptr[2] = "Invalid input parameter"; break;
                case MCU_E_UNINIT: ErrorString_Ptr[2] = "Driver uninitialized"; break;
                case MCU_E_ALREADY_INITIALIZED: ErrorString_Ptr[2] = "Driver already initialized"; break;
                default: break; //Error ID not registered.
            }
        break;

        case CDD_PLATFORM_MODULE_ID: //3 errors.
            switch ( ErrorId ) {
                case PLATFORM_E_PARAM_POINTER: ErrorString_Ptr[2] = "Invalid pointer"; break;
                case PLATFORM_E_PARAM_CONFIG: ErrorString_Ptr[2] = "Call from wrong mapped partition"; break;
                case PLATFORM_E_PARAM_OUT_OF_RANGE: ErrorString_Ptr[2] = "Parameter out of range"; break;
                default: break; //Error ID not registered.
            }
        break;

        case PORT_MODULE_ID: //7 errors.
            switch ( ErrorId ) {
                case PORT_E_PARAM_PIN: ErrorString_Ptr[2] = "Invalid Port Pin ID"; break;
                case PORT_E_MODE_UNCHANGEABLE: ErrorString_Ptr[2] = "Port pin mode is unchangeable"; break;
                case PORT_E_INIT_FAILED: ErrorString_Ptr[2] = "Invalid conf pointer"; break;
                case PORT_E_PARAM_CONFIG: ErrorString_Ptr[2] = "Invalid conf pointer"; break;
                case PORT_E_PARAM_INVALID_MODE: ErrorString_Ptr[2] = "Invalid port pin mode"; break;
                case PORT_E_UNINIT: ErrorString_Ptr[2] = "Driver uninitialized"; break;
                case PORT_E_PARAM_POINTER: ErrorString_Ptr[2] = "Invalid input parameter"; break;
                default: break; //Error ID not registered.
            }
        break;

        case DIO_MODULE_ID: //6 errors.
            switch ( ErrorId ) {
                case DIO_E_PARAM_LEVEL: ErrorString_Ptr[2] = "Invalid dio channel level"; break;
                case DIO_E_PARAM_POINTER: ErrorString_Ptr[2] = "Invalid input parameter"; break;
                case DIO_E_PARAM_INVALID_CHANNEL_ID: ErrorString_Ptr[2] = "Invalid dio channel id"; break;
                case DIO_E_PARAM_INVALID_PORT_ID: ErrorString_Ptr[2] = "Invalid port id"; break;
                case DIO_E_PARAM_INVALID_GROUP_ID: ErrorString_Ptr[2] = "Invalid channel group id"; break;
                case DIO_E_PARAM_CONFIG: ErrorString_Ptr[2] = "Invalid config parameter"; break;
                default: break; //Error ID not registered.
            }
        break;        

        case PWM_MODULE_ID: //6 errors.
            switch ( ErrorId ) {
                case PWM_E_INIT_FAILED: ErrorString_Ptr[2] = "Invalid config pointer"; break;
                case PWM_E_UNINIT: ErrorString_Ptr[2] = "Driver uninitialized"; break;
                case PWM_E_ALREADY_INITIALIZED: ErrorString_Ptr[2] = "Driver already initialized"; break;
                case PWM_E_PARAM_CHANNEL: ErrorString_Ptr[2] = "Invalid pwm channel"; break;
                case PWM_E_PERIOD_UNCHANGEABLE: ErrorString_Ptr[2] = "Pwm channel period is unchangeable"; break;
                case PWM_E_PARAM_POINTER: ErrorString_Ptr[2] = "Invalid input parameter"; break;
                default: break; //Error ID not registered.
            }
        break;

        case ADC_MODULE_ID: //6 errors.
            switch ( ErrorId ) {
                case ADC_E_UNINIT: ErrorString_Ptr[2] = "Driver uninitialized"; break;
                case ADC_E_ALREADY_INITIALIZED: ErrorString_Ptr[2] = "Driver already initialized"; break;
                case ADC_E_PARAM_POINTER: ErrorString_Ptr[2] = "Invalid pointer"; break;
                case ADC_E_PARAM_GROUP: ErrorString_Ptr[2] = "Invalid adc group"; break;
                case ADC_E_BUFFER_UNINIT: ErrorString_Ptr[2] = "Buffer pointer not initialized"; break;
                case ADC_E_TIMEOUT: ErrorString_Ptr[2] = "Calibration operation timed out"; break;
                default: break; //Error ID not registered.
            }
        break;

        default:    //Module Id is no used in aplication.
        break;
    }

    return ErrorString_Ptr[2];
}

/*================================================================================================*/
/**
* @brief   This function provides the service for reporting of development errors.
* @details This is a function stub only. It only loads the global IDs with the IDs of the latest error that occured
*
* @param[in]     ModuleId    Module ID of Calling Module
* @param[in]     InstanceId  Index of Module starting at 0
* @param[in]     ApiId       ID of API with Error
* @param[in]     ErrorId     ID of Error
*
* @requirement DET009
*/
Std_ReturnType Det_ReportError(uint16 ModuleId,
                               uint8 InstanceId,
                               uint8 ApiId,
                               uint8 ErrorId)
{
    //local data.
    char* Strings_error[3]; //Pointer to strings for the error message.

    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    Det_ModuleId[u32CoreId] = ModuleId;
    Det_InstanceId[u32CoreId] = InstanceId;
    Det_ApiId[u32CoreId] = ApiId;
    Det_ErrorId[u32CoreId] = ErrorId;

    //Obtaining strings for error message.
    Strings_error[0] = Det_GetModuleName( ModuleId );
    Strings_error[1] = Det_GetApiName( ModuleId, ApiId );
    Strings_error[2] = Det_GetErrorMeaning( ModuleId, ErrorId );

    SEGGER_RTT_printf( 0, "Error: Error in %s in function %s with the instance %d, %s\n", 
    Strings_error[0], Strings_error[1], InstanceId, Strings_error[2] );

    return E_OK;
}

/*================================================================================================*/
/**
* @brief   This function provides the service for reporting of runtime errors.
* @details This is a function stub only. It only loads the global IDs with the IDs of the latest error that occured
*
* @param[in]     ModuleId    Module ID of Calling Module
* @param[in]     InstanceId  Index of Module starting at 0
* @param[in]     ApiId       ID of API with Error
* @param[in]     ErrorId     ID of Error
*
* @requirement SWS_Det_01001
*/
Std_ReturnType Det_ReportRuntimeError(uint16 ModuleId,
                                      uint8 InstanceId,
                                      uint8 ApiId,
                                      uint8 ErrorId)
{
    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    Det_RuntimeModuleId[u32CoreId] = ModuleId;
    Det_RuntimeInstanceId[u32CoreId] = InstanceId;
    Det_RuntimeApiId[u32CoreId] = ApiId;
    Det_RuntimeErrorId[u32CoreId] = ErrorId;

    return E_OK;
}

/*================================================================================================*/
/**
* @brief   This function provides the service for reporting of transient errors.
* @details This is a function stub only. It only loads the global IDs with the IDs of the latest error that occured
*
* @param[in]     ModuleId    Module ID of Calling Module
* @param[in]     InstanceId  Index of Module starting at 0
* @param[in]     ApiId       ID of API with Error
* @param[in]     FaultId     ID of Fault
*
* @requirement SWS_Det_01001
*/
Std_ReturnType Det_ReportTransientFault(uint16 ModuleId,
                                        uint8 InstanceId,
                                        uint8 ApiId,
                                        uint8 FaultId)
{
    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    Det_TransientModuleId[u32CoreId] = ModuleId;
    Det_TransientInstanceId[u32CoreId] = InstanceId;
    Det_TransientApiId[u32CoreId] = ApiId;
    Det_TransientFaultId[u32CoreId] = FaultId;

    return E_OK;
}

/*================================================================================================*/
/**
* @brief   This function provides the starting after the Det_Init has been called.
* @details This is a function stub only.  Det_Init and Det_Start shall always be available.
*          Functionality is void in this implementation
*
* @requirement DET010
*/
void Det_Start(void)
{
    /* Do nothing */
}

#define DET_STOP_SEC_CODE
#include "Det_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */
