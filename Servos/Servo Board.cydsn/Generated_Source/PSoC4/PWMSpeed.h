/*******************************************************************************
* File Name: PWMSpeed.h
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

#if !defined(CY_PWM_PWMSpeed_H)
#define CY_PWM_PWMSpeed_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 PWMSpeed_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define PWMSpeed_Resolution                     (16u)
#define PWMSpeed_UsingFixedFunction             (0u)
#define PWMSpeed_DeadBandMode                   (0u)
#define PWMSpeed_KillModeMinTime                (0u)
#define PWMSpeed_KillMode                       (0u)
#define PWMSpeed_PWMMode                        (1u)
#define PWMSpeed_PWMModeIsCenterAligned         (0u)
#define PWMSpeed_DeadBandUsed                   (0u)
#define PWMSpeed_DeadBand2_4                    (0u)

#if !defined(PWMSpeed_PWMUDB_genblk8_stsreg__REMOVED)
    #define PWMSpeed_UseStatus                  (0u)
#else
    #define PWMSpeed_UseStatus                  (0u)
#endif /* !defined(PWMSpeed_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(PWMSpeed_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define PWMSpeed_UseControl                 (1u)
#else
    #define PWMSpeed_UseControl                 (0u)
#endif /* !defined(PWMSpeed_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define PWMSpeed_UseOneCompareMode              (0u)
#define PWMSpeed_MinimumKillTime                (1u)
#define PWMSpeed_EnableMode                     (0u)

#define PWMSpeed_CompareMode1SW                 (0u)
#define PWMSpeed_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define PWMSpeed__B_PWM__DISABLED 0
#define PWMSpeed__B_PWM__ASYNCHRONOUS 1
#define PWMSpeed__B_PWM__SINGLECYCLE 2
#define PWMSpeed__B_PWM__LATCHED 3
#define PWMSpeed__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define PWMSpeed__B_PWM__DBMDISABLED 0
#define PWMSpeed__B_PWM__DBM_2_4_CLOCKS 1
#define PWMSpeed__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define PWMSpeed__B_PWM__ONE_OUTPUT 0
#define PWMSpeed__B_PWM__TWO_OUTPUTS 1
#define PWMSpeed__B_PWM__DUAL_EDGE 2
#define PWMSpeed__B_PWM__CENTER_ALIGN 3
#define PWMSpeed__B_PWM__DITHER 5
#define PWMSpeed__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define PWMSpeed__B_PWM__LESS_THAN 1
#define PWMSpeed__B_PWM__LESS_THAN_OR_EQUAL 2
#define PWMSpeed__B_PWM__GREATER_THAN 3
#define PWMSpeed__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define PWMSpeed__B_PWM__EQUAL 0
#define PWMSpeed__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!PWMSpeed_UsingFixedFunction)
        uint16 PWMUdb;               /* PWM Current Counter value  */
        #if(!PWMSpeed_PWMModeIsCenterAligned)
            uint16 PWMPeriod;
        #endif /* (!PWMSpeed_PWMModeIsCenterAligned) */
        #if (PWMSpeed_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (PWMSpeed_UseStatus) */

        /* Backup for Deadband parameters */
        #if(PWMSpeed_DeadBandMode == PWMSpeed__B_PWM__DBM_256_CLOCKS || \
            PWMSpeed_DeadBandMode == PWMSpeed__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(PWMSpeed_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (PWMSpeed_KillModeMinTime) */

        /* Backup control register */
        #if(PWMSpeed_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (PWMSpeed_UseControl) */

    #endif /* (!PWMSpeed_UsingFixedFunction) */

}PWMSpeed_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    PWMSpeed_Start(void) ;
void    PWMSpeed_Stop(void) ;

#if (PWMSpeed_UseStatus || PWMSpeed_UsingFixedFunction)
    void  PWMSpeed_SetInterruptMode(uint8 interruptMode) ;
    uint8 PWMSpeed_ReadStatusRegister(void) ;
#endif /* (PWMSpeed_UseStatus || PWMSpeed_UsingFixedFunction) */

#define PWMSpeed_GetInterruptSource() PWMSpeed_ReadStatusRegister()

#if (PWMSpeed_UseControl)
    uint8 PWMSpeed_ReadControlRegister(void) ;
    void  PWMSpeed_WriteControlRegister(uint8 control)
          ;
#endif /* (PWMSpeed_UseControl) */

#if (PWMSpeed_UseOneCompareMode)
   #if (PWMSpeed_CompareMode1SW)
       void    PWMSpeed_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (PWMSpeed_CompareMode1SW) */
#else
    #if (PWMSpeed_CompareMode1SW)
        void    PWMSpeed_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (PWMSpeed_CompareMode1SW) */
    #if (PWMSpeed_CompareMode2SW)
        void    PWMSpeed_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (PWMSpeed_CompareMode2SW) */
#endif /* (PWMSpeed_UseOneCompareMode) */

#if (!PWMSpeed_UsingFixedFunction)
    uint16   PWMSpeed_ReadCounter(void) ;
    uint16 PWMSpeed_ReadCapture(void) ;

    #if (PWMSpeed_UseStatus)
            void PWMSpeed_ClearFIFO(void) ;
    #endif /* (PWMSpeed_UseStatus) */

    void    PWMSpeed_WriteCounter(uint16 counter)
            ;
#endif /* (!PWMSpeed_UsingFixedFunction) */

void    PWMSpeed_WritePeriod(uint16 period)
        ;
uint16 PWMSpeed_ReadPeriod(void) ;

#if (PWMSpeed_UseOneCompareMode)
    void    PWMSpeed_WriteCompare(uint16 compare)
            ;
    uint16 PWMSpeed_ReadCompare(void) ;
#else
    void    PWMSpeed_WriteCompare1(uint16 compare)
            ;
    uint16 PWMSpeed_ReadCompare1(void) ;
    void    PWMSpeed_WriteCompare2(uint16 compare)
            ;
    uint16 PWMSpeed_ReadCompare2(void) ;
#endif /* (PWMSpeed_UseOneCompareMode) */


#if (PWMSpeed_DeadBandUsed)
    void    PWMSpeed_WriteDeadTime(uint8 deadtime) ;
    uint8   PWMSpeed_ReadDeadTime(void) ;
#endif /* (PWMSpeed_DeadBandUsed) */

#if ( PWMSpeed_KillModeMinTime)
    void PWMSpeed_WriteKillTime(uint8 killtime) ;
    uint8 PWMSpeed_ReadKillTime(void) ;
#endif /* ( PWMSpeed_KillModeMinTime) */

void PWMSpeed_Init(void) ;
void PWMSpeed_Enable(void) ;
void PWMSpeed_Sleep(void) ;
void PWMSpeed_Wakeup(void) ;
void PWMSpeed_SaveConfig(void) ;
void PWMSpeed_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define PWMSpeed_INIT_PERIOD_VALUE          (4999u)
#define PWMSpeed_INIT_COMPARE_VALUE1        (550u)
#define PWMSpeed_INIT_COMPARE_VALUE2        (550u)
#define PWMSpeed_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    PWMSpeed_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    PWMSpeed_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    PWMSpeed_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    PWMSpeed_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define PWMSpeed_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  PWMSpeed_CTRL_CMPMODE2_SHIFT)
#define PWMSpeed_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  PWMSpeed_CTRL_CMPMODE1_SHIFT)
#define PWMSpeed_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (PWMSpeed_UsingFixedFunction)
   #define PWMSpeed_PERIOD_LSB              (*(reg16 *) PWMSpeed_PWMHW__PER0)
   #define PWMSpeed_PERIOD_LSB_PTR          ( (reg16 *) PWMSpeed_PWMHW__PER0)
   #define PWMSpeed_COMPARE1_LSB            (*(reg16 *) PWMSpeed_PWMHW__CNT_CMP0)
   #define PWMSpeed_COMPARE1_LSB_PTR        ( (reg16 *) PWMSpeed_PWMHW__CNT_CMP0)
   #define PWMSpeed_COMPARE2_LSB            (0x00u)
   #define PWMSpeed_COMPARE2_LSB_PTR        (0x00u)
   #define PWMSpeed_COUNTER_LSB             (*(reg16 *) PWMSpeed_PWMHW__CNT_CMP0)
   #define PWMSpeed_COUNTER_LSB_PTR         ( (reg16 *) PWMSpeed_PWMHW__CNT_CMP0)
   #define PWMSpeed_CAPTURE_LSB             (*(reg16 *) PWMSpeed_PWMHW__CAP0)
   #define PWMSpeed_CAPTURE_LSB_PTR         ( (reg16 *) PWMSpeed_PWMHW__CAP0)
   #define PWMSpeed_RT1                     (*(reg8 *)  PWMSpeed_PWMHW__RT1)
   #define PWMSpeed_RT1_PTR                 ( (reg8 *)  PWMSpeed_PWMHW__RT1)

#else
   #if (PWMSpeed_Resolution == 8u) /* 8bit - PWM */

       #if(PWMSpeed_PWMModeIsCenterAligned)
           #define PWMSpeed_PERIOD_LSB      (*(reg8 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__D1_REG)
           #define PWMSpeed_PERIOD_LSB_PTR  ((reg8 *)   PWMSpeed_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #else
           #define PWMSpeed_PERIOD_LSB      (*(reg8 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__F0_REG)
           #define PWMSpeed_PERIOD_LSB_PTR  ((reg8 *)   PWMSpeed_PWMUDB_sP16_pwmdp_u0__F0_REG)
       #endif /* (PWMSpeed_PWMModeIsCenterAligned) */

       #define PWMSpeed_COMPARE1_LSB        (*(reg8 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define PWMSpeed_COMPARE1_LSB_PTR    ((reg8 *)   PWMSpeed_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define PWMSpeed_COMPARE2_LSB        (*(reg8 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define PWMSpeed_COMPARE2_LSB_PTR    ((reg8 *)   PWMSpeed_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define PWMSpeed_COUNTERCAP_LSB      (*(reg8 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define PWMSpeed_COUNTERCAP_LSB_PTR  ((reg8 *)   PWMSpeed_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define PWMSpeed_COUNTER_LSB         (*(reg8 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define PWMSpeed_COUNTER_LSB_PTR     ((reg8 *)   PWMSpeed_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define PWMSpeed_CAPTURE_LSB         (*(reg8 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__F1_REG)
       #define PWMSpeed_CAPTURE_LSB_PTR     ((reg8 *)   PWMSpeed_PWMUDB_sP16_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(PWMSpeed_PWMModeIsCenterAligned)
               #define PWMSpeed_PERIOD_LSB      (*(reg16 *) PWMSpeed_PWMUDB_sP16_pwmdp_u0__D1_REG)
               #define PWMSpeed_PERIOD_LSB_PTR  ((reg16 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #else
               #define PWMSpeed_PERIOD_LSB      (*(reg16 *) PWMSpeed_PWMUDB_sP16_pwmdp_u0__F0_REG)
               #define PWMSpeed_PERIOD_LSB_PTR  ((reg16 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__F0_REG)
            #endif /* (PWMSpeed_PWMModeIsCenterAligned) */

            #define PWMSpeed_COMPARE1_LSB       (*(reg16 *) PWMSpeed_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define PWMSpeed_COMPARE1_LSB_PTR   ((reg16 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define PWMSpeed_COMPARE2_LSB       (*(reg16 *) PWMSpeed_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define PWMSpeed_COMPARE2_LSB_PTR   ((reg16 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define PWMSpeed_COUNTERCAP_LSB     (*(reg16 *) PWMSpeed_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define PWMSpeed_COUNTERCAP_LSB_PTR ((reg16 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define PWMSpeed_COUNTER_LSB        (*(reg16 *) PWMSpeed_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define PWMSpeed_COUNTER_LSB_PTR    ((reg16 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define PWMSpeed_CAPTURE_LSB        (*(reg16 *) PWMSpeed_PWMUDB_sP16_pwmdp_u0__F1_REG)
            #define PWMSpeed_CAPTURE_LSB_PTR    ((reg16 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__F1_REG)
        #else
            #if(PWMSpeed_PWMModeIsCenterAligned)
               #define PWMSpeed_PERIOD_LSB      (*(reg16 *) PWMSpeed_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
               #define PWMSpeed_PERIOD_LSB_PTR  ((reg16 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #else
               #define PWMSpeed_PERIOD_LSB      (*(reg16 *) PWMSpeed_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
               #define PWMSpeed_PERIOD_LSB_PTR  ((reg16 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
            #endif /* (PWMSpeed_PWMModeIsCenterAligned) */

            #define PWMSpeed_COMPARE1_LSB       (*(reg16 *) PWMSpeed_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define PWMSpeed_COMPARE1_LSB_PTR   ((reg16 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define PWMSpeed_COMPARE2_LSB       (*(reg16 *) PWMSpeed_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define PWMSpeed_COMPARE2_LSB_PTR   ((reg16 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define PWMSpeed_COUNTERCAP_LSB     (*(reg16 *) PWMSpeed_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define PWMSpeed_COUNTERCAP_LSB_PTR ((reg16 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define PWMSpeed_COUNTER_LSB        (*(reg16 *) PWMSpeed_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define PWMSpeed_COUNTER_LSB_PTR    ((reg16 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define PWMSpeed_CAPTURE_LSB        (*(reg16 *) PWMSpeed_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
            #define PWMSpeed_CAPTURE_LSB_PTR    ((reg16 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define PWMSpeed_AUX_CONTROLDP1          (*(reg8 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)
       #define PWMSpeed_AUX_CONTROLDP1_PTR      ((reg8 *)   PWMSpeed_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (PWMSpeed_Resolution == 8) */

   #define PWMSpeed_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__A1_REG)
   #define PWMSpeed_AUX_CONTROLDP0          (*(reg8 *)  PWMSpeed_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)
   #define PWMSpeed_AUX_CONTROLDP0_PTR      ((reg8 *)   PWMSpeed_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (PWMSpeed_UsingFixedFunction) */

#if(PWMSpeed_KillModeMinTime )
    #define PWMSpeed_KILLMODEMINTIME        (*(reg8 *)  PWMSpeed_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define PWMSpeed_KILLMODEMINTIME_PTR    ((reg8 *)   PWMSpeed_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (PWMSpeed_KillModeMinTime ) */

#if(PWMSpeed_DeadBandMode == PWMSpeed__B_PWM__DBM_256_CLOCKS)
    #define PWMSpeed_DEADBAND_COUNT         (*(reg8 *)  PWMSpeed_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define PWMSpeed_DEADBAND_COUNT_PTR     ((reg8 *)   PWMSpeed_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define PWMSpeed_DEADBAND_LSB_PTR       ((reg8 *)   PWMSpeed_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define PWMSpeed_DEADBAND_LSB           (*(reg8 *)  PWMSpeed_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(PWMSpeed_DeadBandMode == PWMSpeed__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (PWMSpeed_UsingFixedFunction)
        #define PWMSpeed_DEADBAND_COUNT         (*(reg8 *)  PWMSpeed_PWMHW__CFG0)
        #define PWMSpeed_DEADBAND_COUNT_PTR     ((reg8 *)   PWMSpeed_PWMHW__CFG0)
        #define PWMSpeed_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << PWMSpeed_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define PWMSpeed_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define PWMSpeed_DEADBAND_COUNT         (*(reg8 *)  PWMSpeed_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define PWMSpeed_DEADBAND_COUNT_PTR     ((reg8 *)   PWMSpeed_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define PWMSpeed_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << PWMSpeed_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define PWMSpeed_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (PWMSpeed_UsingFixedFunction) */
#endif /* (PWMSpeed_DeadBandMode == PWMSpeed__B_PWM__DBM_256_CLOCKS) */



#if (PWMSpeed_UsingFixedFunction)
    #define PWMSpeed_STATUS                 (*(reg8 *) PWMSpeed_PWMHW__SR0)
    #define PWMSpeed_STATUS_PTR             ((reg8 *) PWMSpeed_PWMHW__SR0)
    #define PWMSpeed_STATUS_MASK            (*(reg8 *) PWMSpeed_PWMHW__SR0)
    #define PWMSpeed_STATUS_MASK_PTR        ((reg8 *) PWMSpeed_PWMHW__SR0)
    #define PWMSpeed_CONTROL                (*(reg8 *) PWMSpeed_PWMHW__CFG0)
    #define PWMSpeed_CONTROL_PTR            ((reg8 *) PWMSpeed_PWMHW__CFG0)
    #define PWMSpeed_CONTROL2               (*(reg8 *) PWMSpeed_PWMHW__CFG1)
    #define PWMSpeed_CONTROL3               (*(reg8 *) PWMSpeed_PWMHW__CFG2)
    #define PWMSpeed_GLOBAL_ENABLE          (*(reg8 *) PWMSpeed_PWMHW__PM_ACT_CFG)
    #define PWMSpeed_GLOBAL_ENABLE_PTR      ( (reg8 *) PWMSpeed_PWMHW__PM_ACT_CFG)
    #define PWMSpeed_GLOBAL_STBY_ENABLE     (*(reg8 *) PWMSpeed_PWMHW__PM_STBY_CFG)
    #define PWMSpeed_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) PWMSpeed_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define PWMSpeed_BLOCK_EN_MASK          (PWMSpeed_PWMHW__PM_ACT_MSK)
    #define PWMSpeed_BLOCK_STBY_EN_MASK     (PWMSpeed_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define PWMSpeed_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define PWMSpeed_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define PWMSpeed_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define PWMSpeed_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define PWMSpeed_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define PWMSpeed_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define PWMSpeed_CTRL_ENABLE            (uint8)((uint8)0x01u << PWMSpeed_CTRL_ENABLE_SHIFT)
    #define PWMSpeed_CTRL_RESET             (uint8)((uint8)0x01u << PWMSpeed_CTRL_RESET_SHIFT)
    #define PWMSpeed_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << PWMSpeed_CTRL_CMPMODE2_SHIFT)
    #define PWMSpeed_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << PWMSpeed_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define PWMSpeed_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define PWMSpeed_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << PWMSpeed_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define PWMSpeed_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define PWMSpeed_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define PWMSpeed_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define PWMSpeed_STATUS_TC_INT_EN_MASK_SHIFT            (PWMSpeed_STATUS_TC_SHIFT - 4u)
    #define PWMSpeed_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define PWMSpeed_STATUS_CMP1_INT_EN_MASK_SHIFT          (PWMSpeed_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define PWMSpeed_STATUS_TC              (uint8)((uint8)0x01u << PWMSpeed_STATUS_TC_SHIFT)
    #define PWMSpeed_STATUS_CMP1            (uint8)((uint8)0x01u << PWMSpeed_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define PWMSpeed_STATUS_TC_INT_EN_MASK              (uint8)((uint8)PWMSpeed_STATUS_TC >> 4u)
    #define PWMSpeed_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)PWMSpeed_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define PWMSpeed_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define PWMSpeed_RT1_MASK              (uint8)((uint8)0x03u << PWMSpeed_RT1_SHIFT)
    #define PWMSpeed_SYNC                  (uint8)((uint8)0x03u << PWMSpeed_RT1_SHIFT)
    #define PWMSpeed_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define PWMSpeed_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << PWMSpeed_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define PWMSpeed_SYNCDSI_EN            (uint8)((uint8)0x0Fu << PWMSpeed_SYNCDSI_SHIFT)


#else
    #define PWMSpeed_STATUS                (*(reg8 *)   PWMSpeed_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define PWMSpeed_STATUS_PTR            ((reg8 *)    PWMSpeed_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define PWMSpeed_STATUS_MASK           (*(reg8 *)   PWMSpeed_PWMUDB_genblk8_stsreg__MASK_REG)
    #define PWMSpeed_STATUS_MASK_PTR       ((reg8 *)    PWMSpeed_PWMUDB_genblk8_stsreg__MASK_REG)
    #define PWMSpeed_STATUS_AUX_CTRL       (*(reg8 *)   PWMSpeed_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define PWMSpeed_CONTROL               (*(reg8 *)   PWMSpeed_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define PWMSpeed_CONTROL_PTR           ((reg8 *)    PWMSpeed_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define PWMSpeed_CTRL_ENABLE_SHIFT      (0x07u)
    #define PWMSpeed_CTRL_RESET_SHIFT       (0x06u)
    #define PWMSpeed_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define PWMSpeed_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define PWMSpeed_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define PWMSpeed_CTRL_ENABLE            (uint8)((uint8)0x01u << PWMSpeed_CTRL_ENABLE_SHIFT)
    #define PWMSpeed_CTRL_RESET             (uint8)((uint8)0x01u << PWMSpeed_CTRL_RESET_SHIFT)
    #define PWMSpeed_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << PWMSpeed_CTRL_CMPMODE2_SHIFT)
    #define PWMSpeed_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << PWMSpeed_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define PWMSpeed_STATUS_KILL_SHIFT          (0x05u)
    #define PWMSpeed_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define PWMSpeed_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define PWMSpeed_STATUS_TC_SHIFT            (0x02u)
    #define PWMSpeed_STATUS_CMP2_SHIFT          (0x01u)
    #define PWMSpeed_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define PWMSpeed_STATUS_KILL_INT_EN_MASK_SHIFT          (PWMSpeed_STATUS_KILL_SHIFT)
    #define PWMSpeed_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (PWMSpeed_STATUS_FIFONEMPTY_SHIFT)
    #define PWMSpeed_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (PWMSpeed_STATUS_FIFOFULL_SHIFT)
    #define PWMSpeed_STATUS_TC_INT_EN_MASK_SHIFT            (PWMSpeed_STATUS_TC_SHIFT)
    #define PWMSpeed_STATUS_CMP2_INT_EN_MASK_SHIFT          (PWMSpeed_STATUS_CMP2_SHIFT)
    #define PWMSpeed_STATUS_CMP1_INT_EN_MASK_SHIFT          (PWMSpeed_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define PWMSpeed_STATUS_KILL            (uint8)((uint8)0x00u << PWMSpeed_STATUS_KILL_SHIFT )
    #define PWMSpeed_STATUS_FIFOFULL        (uint8)((uint8)0x01u << PWMSpeed_STATUS_FIFOFULL_SHIFT)
    #define PWMSpeed_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << PWMSpeed_STATUS_FIFONEMPTY_SHIFT)
    #define PWMSpeed_STATUS_TC              (uint8)((uint8)0x01u << PWMSpeed_STATUS_TC_SHIFT)
    #define PWMSpeed_STATUS_CMP2            (uint8)((uint8)0x01u << PWMSpeed_STATUS_CMP2_SHIFT)
    #define PWMSpeed_STATUS_CMP1            (uint8)((uint8)0x01u << PWMSpeed_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define PWMSpeed_STATUS_KILL_INT_EN_MASK            (PWMSpeed_STATUS_KILL)
    #define PWMSpeed_STATUS_FIFOFULL_INT_EN_MASK        (PWMSpeed_STATUS_FIFOFULL)
    #define PWMSpeed_STATUS_FIFONEMPTY_INT_EN_MASK      (PWMSpeed_STATUS_FIFONEMPTY)
    #define PWMSpeed_STATUS_TC_INT_EN_MASK              (PWMSpeed_STATUS_TC)
    #define PWMSpeed_STATUS_CMP2_INT_EN_MASK            (PWMSpeed_STATUS_CMP2)
    #define PWMSpeed_STATUS_CMP1_INT_EN_MASK            (PWMSpeed_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define PWMSpeed_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define PWMSpeed_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define PWMSpeed_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define PWMSpeed_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define PWMSpeed_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* PWMSpeed_UsingFixedFunction */

#endif  /* CY_PWM_PWMSpeed_H */


/* [] END OF FILE */
