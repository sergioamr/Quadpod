/*******************************************************************************
* File Name: PWMB.h
* Version 3.30
*
* Description:
*  Contains the prototypes and constants for the functions available to the
*  PWM user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PWM_PWMB_H)
#define CY_PWM_PWMB_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 PWMB_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define PWMB_Resolution                     (16u)
#define PWMB_UsingFixedFunction             (0u)
#define PWMB_DeadBandMode                   (1u)
#define PWMB_KillModeMinTime                (0u)
#define PWMB_KillMode                       (0u)
#define PWMB_PWMMode                        (1u)
#define PWMB_PWMModeIsCenterAligned         (0u)
#define PWMB_DeadBandUsed                   (1u)
#define PWMB_DeadBand2_4                    (1u)

#if !defined(PWMB_PWMUDB_genblk8_stsreg__REMOVED)
    #define PWMB_UseStatus                  (1u)
#else
    #define PWMB_UseStatus                  (0u)
#endif /* !defined(PWMB_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(PWMB_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define PWMB_UseControl                 (1u)
#else
    #define PWMB_UseControl                 (0u)
#endif /* !defined(PWMB_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define PWMB_UseOneCompareMode              (0u)
#define PWMB_MinimumKillTime                (1u)
#define PWMB_EnableMode                     (0u)

#define PWMB_CompareMode1SW                 (0u)
#define PWMB_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define PWMB__B_PWM__DISABLED 0
#define PWMB__B_PWM__ASYNCHRONOUS 1
#define PWMB__B_PWM__SINGLECYCLE 2
#define PWMB__B_PWM__LATCHED 3
#define PWMB__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define PWMB__B_PWM__DBMDISABLED 0
#define PWMB__B_PWM__DBM_2_4_CLOCKS 1
#define PWMB__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define PWMB__B_PWM__ONE_OUTPUT 0
#define PWMB__B_PWM__TWO_OUTPUTS 1
#define PWMB__B_PWM__DUAL_EDGE 2
#define PWMB__B_PWM__CENTER_ALIGN 3
#define PWMB__B_PWM__DITHER 5
#define PWMB__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define PWMB__B_PWM__LESS_THAN 1
#define PWMB__B_PWM__LESS_THAN_OR_EQUAL 2
#define PWMB__B_PWM__GREATER_THAN 3
#define PWMB__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define PWMB__B_PWM__EQUAL 0
#define PWMB__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!PWMB_UsingFixedFunction)
        uint16 PWMUdb;               /* PWM Current Counter value  */
        #if(!PWMB_PWMModeIsCenterAligned)
            uint16 PWMPeriod;
        #endif /* (!PWMB_PWMModeIsCenterAligned) */
        #if (PWMB_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (PWMB_UseStatus) */

        /* Backup for Deadband parameters */
        #if(PWMB_DeadBandMode == PWMB__B_PWM__DBM_256_CLOCKS || \
            PWMB_DeadBandMode == PWMB__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(PWMB_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (PWMB_KillModeMinTime) */

        /* Backup control register */
        #if(PWMB_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (PWMB_UseControl) */

    #endif /* (!PWMB_UsingFixedFunction) */

}PWMB_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    PWMB_Start(void) ;
void    PWMB_Stop(void) ;

#if (PWMB_UseStatus || PWMB_UsingFixedFunction)
    void  PWMB_SetInterruptMode(uint8 interruptMode) ;
    uint8 PWMB_ReadStatusRegister(void) ;
#endif /* (PWMB_UseStatus || PWMB_UsingFixedFunction) */

#define PWMB_GetInterruptSource() PWMB_ReadStatusRegister()

#if (PWMB_UseControl)
    uint8 PWMB_ReadControlRegister(void) ;
    void  PWMB_WriteControlRegister(uint8 control)
          ;
#endif /* (PWMB_UseControl) */

#if (PWMB_UseOneCompareMode)
   #if (PWMB_CompareMode1SW)
       void    PWMB_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (PWMB_CompareMode1SW) */
#else
    #if (PWMB_CompareMode1SW)
        void    PWMB_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (PWMB_CompareMode1SW) */
    #if (PWMB_CompareMode2SW)
        void    PWMB_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (PWMB_CompareMode2SW) */
#endif /* (PWMB_UseOneCompareMode) */

#if (!PWMB_UsingFixedFunction)
    uint16   PWMB_ReadCounter(void) ;
    uint16 PWMB_ReadCapture(void) ;

    #if (PWMB_UseStatus)
            void PWMB_ClearFIFO(void) ;
    #endif /* (PWMB_UseStatus) */

    void    PWMB_WriteCounter(uint16 counter)
            ;
#endif /* (!PWMB_UsingFixedFunction) */

void    PWMB_WritePeriod(uint16 period)
        ;
uint16 PWMB_ReadPeriod(void) ;

#if (PWMB_UseOneCompareMode)
    void    PWMB_WriteCompare(uint16 compare)
            ;
    uint16 PWMB_ReadCompare(void) ;
#else
    void    PWMB_WriteCompare1(uint16 compare)
            ;
    uint16 PWMB_ReadCompare1(void) ;
    void    PWMB_WriteCompare2(uint16 compare)
            ;
    uint16 PWMB_ReadCompare2(void) ;
#endif /* (PWMB_UseOneCompareMode) */


#if (PWMB_DeadBandUsed)
    void    PWMB_WriteDeadTime(uint8 deadtime) ;
    uint8   PWMB_ReadDeadTime(void) ;
#endif /* (PWMB_DeadBandUsed) */

#if ( PWMB_KillModeMinTime)
    void PWMB_WriteKillTime(uint8 killtime) ;
    uint8 PWMB_ReadKillTime(void) ;
#endif /* ( PWMB_KillModeMinTime) */

void PWMB_Init(void) ;
void PWMB_Enable(void) ;
void PWMB_Sleep(void) ;
void PWMB_Wakeup(void) ;
void PWMB_SaveConfig(void) ;
void PWMB_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define PWMB_INIT_PERIOD_VALUE          (63998u)
#define PWMB_INIT_COMPARE_VALUE1        (4800u)
#define PWMB_INIT_COMPARE_VALUE2        (4800u)
#define PWMB_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    PWMB_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    PWMB_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    PWMB_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    PWMB_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define PWMB_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  PWMB_CTRL_CMPMODE2_SHIFT)
#define PWMB_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  PWMB_CTRL_CMPMODE1_SHIFT)
#define PWMB_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (PWMB_UsingFixedFunction)
   #define PWMB_PERIOD_LSB              (*(reg16 *) PWMB_PWMHW__PER0)
   #define PWMB_PERIOD_LSB_PTR          ( (reg16 *) PWMB_PWMHW__PER0)
   #define PWMB_COMPARE1_LSB            (*(reg16 *) PWMB_PWMHW__CNT_CMP0)
   #define PWMB_COMPARE1_LSB_PTR        ( (reg16 *) PWMB_PWMHW__CNT_CMP0)
   #define PWMB_COMPARE2_LSB            (0x00u)
   #define PWMB_COMPARE2_LSB_PTR        (0x00u)
   #define PWMB_COUNTER_LSB             (*(reg16 *) PWMB_PWMHW__CNT_CMP0)
   #define PWMB_COUNTER_LSB_PTR         ( (reg16 *) PWMB_PWMHW__CNT_CMP0)
   #define PWMB_CAPTURE_LSB             (*(reg16 *) PWMB_PWMHW__CAP0)
   #define PWMB_CAPTURE_LSB_PTR         ( (reg16 *) PWMB_PWMHW__CAP0)
   #define PWMB_RT1                     (*(reg8 *)  PWMB_PWMHW__RT1)
   #define PWMB_RT1_PTR                 ( (reg8 *)  PWMB_PWMHW__RT1)

#else
   #if (PWMB_Resolution == 8u) /* 8bit - PWM */

       #if(PWMB_PWMModeIsCenterAligned)
           #define PWMB_PERIOD_LSB      (*(reg8 *)  PWMB_PWMUDB_sP16_pwmdp_u0__D1_REG)
           #define PWMB_PERIOD_LSB_PTR  ((reg8 *)   PWMB_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #else
           #define PWMB_PERIOD_LSB      (*(reg8 *)  PWMB_PWMUDB_sP16_pwmdp_u0__F0_REG)
           #define PWMB_PERIOD_LSB_PTR  ((reg8 *)   PWMB_PWMUDB_sP16_pwmdp_u0__F0_REG)
       #endif /* (PWMB_PWMModeIsCenterAligned) */

       #define PWMB_COMPARE1_LSB        (*(reg8 *)  PWMB_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define PWMB_COMPARE1_LSB_PTR    ((reg8 *)   PWMB_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define PWMB_COMPARE2_LSB        (*(reg8 *)  PWMB_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define PWMB_COMPARE2_LSB_PTR    ((reg8 *)   PWMB_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define PWMB_COUNTERCAP_LSB      (*(reg8 *)  PWMB_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define PWMB_COUNTERCAP_LSB_PTR  ((reg8 *)   PWMB_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define PWMB_COUNTER_LSB         (*(reg8 *)  PWMB_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define PWMB_COUNTER_LSB_PTR     ((reg8 *)   PWMB_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define PWMB_CAPTURE_LSB         (*(reg8 *)  PWMB_PWMUDB_sP16_pwmdp_u0__F1_REG)
       #define PWMB_CAPTURE_LSB_PTR     ((reg8 *)   PWMB_PWMUDB_sP16_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(PWMB_PWMModeIsCenterAligned)
               #define PWMB_PERIOD_LSB      (*(reg16 *) PWMB_PWMUDB_sP16_pwmdp_u0__D1_REG)
               #define PWMB_PERIOD_LSB_PTR  ((reg16 *)  PWMB_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #else
               #define PWMB_PERIOD_LSB      (*(reg16 *) PWMB_PWMUDB_sP16_pwmdp_u0__F0_REG)
               #define PWMB_PERIOD_LSB_PTR  ((reg16 *)  PWMB_PWMUDB_sP16_pwmdp_u0__F0_REG)
            #endif /* (PWMB_PWMModeIsCenterAligned) */

            #define PWMB_COMPARE1_LSB       (*(reg16 *) PWMB_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define PWMB_COMPARE1_LSB_PTR   ((reg16 *)  PWMB_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define PWMB_COMPARE2_LSB       (*(reg16 *) PWMB_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define PWMB_COMPARE2_LSB_PTR   ((reg16 *)  PWMB_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define PWMB_COUNTERCAP_LSB     (*(reg16 *) PWMB_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define PWMB_COUNTERCAP_LSB_PTR ((reg16 *)  PWMB_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define PWMB_COUNTER_LSB        (*(reg16 *) PWMB_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define PWMB_COUNTER_LSB_PTR    ((reg16 *)  PWMB_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define PWMB_CAPTURE_LSB        (*(reg16 *) PWMB_PWMUDB_sP16_pwmdp_u0__F1_REG)
            #define PWMB_CAPTURE_LSB_PTR    ((reg16 *)  PWMB_PWMUDB_sP16_pwmdp_u0__F1_REG)
        #else
            #if(PWMB_PWMModeIsCenterAligned)
               #define PWMB_PERIOD_LSB      (*(reg16 *) PWMB_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
               #define PWMB_PERIOD_LSB_PTR  ((reg16 *)  PWMB_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #else
               #define PWMB_PERIOD_LSB      (*(reg16 *) PWMB_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
               #define PWMB_PERIOD_LSB_PTR  ((reg16 *)  PWMB_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
            #endif /* (PWMB_PWMModeIsCenterAligned) */

            #define PWMB_COMPARE1_LSB       (*(reg16 *) PWMB_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define PWMB_COMPARE1_LSB_PTR   ((reg16 *)  PWMB_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define PWMB_COMPARE2_LSB       (*(reg16 *) PWMB_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define PWMB_COMPARE2_LSB_PTR   ((reg16 *)  PWMB_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define PWMB_COUNTERCAP_LSB     (*(reg16 *) PWMB_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define PWMB_COUNTERCAP_LSB_PTR ((reg16 *)  PWMB_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define PWMB_COUNTER_LSB        (*(reg16 *) PWMB_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define PWMB_COUNTER_LSB_PTR    ((reg16 *)  PWMB_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define PWMB_CAPTURE_LSB        (*(reg16 *) PWMB_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
            #define PWMB_CAPTURE_LSB_PTR    ((reg16 *)  PWMB_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define PWMB_AUX_CONTROLDP1          (*(reg8 *)  PWMB_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)
       #define PWMB_AUX_CONTROLDP1_PTR      ((reg8 *)   PWMB_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (PWMB_Resolution == 8) */

   #define PWMB_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  PWMB_PWMUDB_sP16_pwmdp_u0__A1_REG)
   #define PWMB_AUX_CONTROLDP0          (*(reg8 *)  PWMB_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)
   #define PWMB_AUX_CONTROLDP0_PTR      ((reg8 *)   PWMB_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (PWMB_UsingFixedFunction) */

#if(PWMB_KillModeMinTime )
    #define PWMB_KILLMODEMINTIME        (*(reg8 *)  PWMB_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define PWMB_KILLMODEMINTIME_PTR    ((reg8 *)   PWMB_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (PWMB_KillModeMinTime ) */

#if(PWMB_DeadBandMode == PWMB__B_PWM__DBM_256_CLOCKS)
    #define PWMB_DEADBAND_COUNT         (*(reg8 *)  PWMB_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define PWMB_DEADBAND_COUNT_PTR     ((reg8 *)   PWMB_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define PWMB_DEADBAND_LSB_PTR       ((reg8 *)   PWMB_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define PWMB_DEADBAND_LSB           (*(reg8 *)  PWMB_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(PWMB_DeadBandMode == PWMB__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (PWMB_UsingFixedFunction)
        #define PWMB_DEADBAND_COUNT         (*(reg8 *)  PWMB_PWMHW__CFG0)
        #define PWMB_DEADBAND_COUNT_PTR     ((reg8 *)   PWMB_PWMHW__CFG0)
        #define PWMB_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << PWMB_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define PWMB_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define PWMB_DEADBAND_COUNT         (*(reg8 *)  PWMB_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define PWMB_DEADBAND_COUNT_PTR     ((reg8 *)   PWMB_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define PWMB_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << PWMB_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define PWMB_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (PWMB_UsingFixedFunction) */
#endif /* (PWMB_DeadBandMode == PWMB__B_PWM__DBM_256_CLOCKS) */



#if (PWMB_UsingFixedFunction)
    #define PWMB_STATUS                 (*(reg8 *) PWMB_PWMHW__SR0)
    #define PWMB_STATUS_PTR             ((reg8 *) PWMB_PWMHW__SR0)
    #define PWMB_STATUS_MASK            (*(reg8 *) PWMB_PWMHW__SR0)
    #define PWMB_STATUS_MASK_PTR        ((reg8 *) PWMB_PWMHW__SR0)
    #define PWMB_CONTROL                (*(reg8 *) PWMB_PWMHW__CFG0)
    #define PWMB_CONTROL_PTR            ((reg8 *) PWMB_PWMHW__CFG0)
    #define PWMB_CONTROL2               (*(reg8 *) PWMB_PWMHW__CFG1)
    #define PWMB_CONTROL3               (*(reg8 *) PWMB_PWMHW__CFG2)
    #define PWMB_GLOBAL_ENABLE          (*(reg8 *) PWMB_PWMHW__PM_ACT_CFG)
    #define PWMB_GLOBAL_ENABLE_PTR      ( (reg8 *) PWMB_PWMHW__PM_ACT_CFG)
    #define PWMB_GLOBAL_STBY_ENABLE     (*(reg8 *) PWMB_PWMHW__PM_STBY_CFG)
    #define PWMB_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) PWMB_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define PWMB_BLOCK_EN_MASK          (PWMB_PWMHW__PM_ACT_MSK)
    #define PWMB_BLOCK_STBY_EN_MASK     (PWMB_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define PWMB_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define PWMB_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define PWMB_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define PWMB_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define PWMB_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define PWMB_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define PWMB_CTRL_ENABLE            (uint8)((uint8)0x01u << PWMB_CTRL_ENABLE_SHIFT)
    #define PWMB_CTRL_RESET             (uint8)((uint8)0x01u << PWMB_CTRL_RESET_SHIFT)
    #define PWMB_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << PWMB_CTRL_CMPMODE2_SHIFT)
    #define PWMB_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << PWMB_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define PWMB_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define PWMB_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << PWMB_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define PWMB_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define PWMB_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define PWMB_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define PWMB_STATUS_TC_INT_EN_MASK_SHIFT            (PWMB_STATUS_TC_SHIFT - 4u)
    #define PWMB_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define PWMB_STATUS_CMP1_INT_EN_MASK_SHIFT          (PWMB_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define PWMB_STATUS_TC              (uint8)((uint8)0x01u << PWMB_STATUS_TC_SHIFT)
    #define PWMB_STATUS_CMP1            (uint8)((uint8)0x01u << PWMB_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define PWMB_STATUS_TC_INT_EN_MASK              (uint8)((uint8)PWMB_STATUS_TC >> 4u)
    #define PWMB_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)PWMB_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define PWMB_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define PWMB_RT1_MASK              (uint8)((uint8)0x03u << PWMB_RT1_SHIFT)
    #define PWMB_SYNC                  (uint8)((uint8)0x03u << PWMB_RT1_SHIFT)
    #define PWMB_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define PWMB_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << PWMB_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define PWMB_SYNCDSI_EN            (uint8)((uint8)0x0Fu << PWMB_SYNCDSI_SHIFT)


#else
    #define PWMB_STATUS                (*(reg8 *)   PWMB_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define PWMB_STATUS_PTR            ((reg8 *)    PWMB_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define PWMB_STATUS_MASK           (*(reg8 *)   PWMB_PWMUDB_genblk8_stsreg__MASK_REG)
    #define PWMB_STATUS_MASK_PTR       ((reg8 *)    PWMB_PWMUDB_genblk8_stsreg__MASK_REG)
    #define PWMB_STATUS_AUX_CTRL       (*(reg8 *)   PWMB_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define PWMB_CONTROL               (*(reg8 *)   PWMB_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define PWMB_CONTROL_PTR           ((reg8 *)    PWMB_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define PWMB_CTRL_ENABLE_SHIFT      (0x07u)
    #define PWMB_CTRL_RESET_SHIFT       (0x06u)
    #define PWMB_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define PWMB_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define PWMB_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define PWMB_CTRL_ENABLE            (uint8)((uint8)0x01u << PWMB_CTRL_ENABLE_SHIFT)
    #define PWMB_CTRL_RESET             (uint8)((uint8)0x01u << PWMB_CTRL_RESET_SHIFT)
    #define PWMB_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << PWMB_CTRL_CMPMODE2_SHIFT)
    #define PWMB_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << PWMB_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define PWMB_STATUS_KILL_SHIFT          (0x05u)
    #define PWMB_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define PWMB_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define PWMB_STATUS_TC_SHIFT            (0x02u)
    #define PWMB_STATUS_CMP2_SHIFT          (0x01u)
    #define PWMB_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define PWMB_STATUS_KILL_INT_EN_MASK_SHIFT          (PWMB_STATUS_KILL_SHIFT)
    #define PWMB_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (PWMB_STATUS_FIFONEMPTY_SHIFT)
    #define PWMB_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (PWMB_STATUS_FIFOFULL_SHIFT)
    #define PWMB_STATUS_TC_INT_EN_MASK_SHIFT            (PWMB_STATUS_TC_SHIFT)
    #define PWMB_STATUS_CMP2_INT_EN_MASK_SHIFT          (PWMB_STATUS_CMP2_SHIFT)
    #define PWMB_STATUS_CMP1_INT_EN_MASK_SHIFT          (PWMB_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define PWMB_STATUS_KILL            (uint8)((uint8)0x00u << PWMB_STATUS_KILL_SHIFT )
    #define PWMB_STATUS_FIFOFULL        (uint8)((uint8)0x01u << PWMB_STATUS_FIFOFULL_SHIFT)
    #define PWMB_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << PWMB_STATUS_FIFONEMPTY_SHIFT)
    #define PWMB_STATUS_TC              (uint8)((uint8)0x01u << PWMB_STATUS_TC_SHIFT)
    #define PWMB_STATUS_CMP2            (uint8)((uint8)0x01u << PWMB_STATUS_CMP2_SHIFT)
    #define PWMB_STATUS_CMP1            (uint8)((uint8)0x01u << PWMB_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define PWMB_STATUS_KILL_INT_EN_MASK            (PWMB_STATUS_KILL)
    #define PWMB_STATUS_FIFOFULL_INT_EN_MASK        (PWMB_STATUS_FIFOFULL)
    #define PWMB_STATUS_FIFONEMPTY_INT_EN_MASK      (PWMB_STATUS_FIFONEMPTY)
    #define PWMB_STATUS_TC_INT_EN_MASK              (PWMB_STATUS_TC)
    #define PWMB_STATUS_CMP2_INT_EN_MASK            (PWMB_STATUS_CMP2)
    #define PWMB_STATUS_CMP1_INT_EN_MASK            (PWMB_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define PWMB_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define PWMB_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define PWMB_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define PWMB_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define PWMB_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* PWMB_UsingFixedFunction */

#endif  /* CY_PWM_PWMB_H */


/* [] END OF FILE */
