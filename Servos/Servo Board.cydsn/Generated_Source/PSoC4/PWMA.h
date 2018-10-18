/*******************************************************************************
* File Name: PWMA.h
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

#if !defined(CY_PWM_PWMA_H)
#define CY_PWM_PWMA_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 PWMA_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define PWMA_Resolution                     (16u)
#define PWMA_UsingFixedFunction             (0u)
#define PWMA_DeadBandMode                   (1u)
#define PWMA_KillModeMinTime                (0u)
#define PWMA_KillMode                       (0u)
#define PWMA_PWMMode                        (1u)
#define PWMA_PWMModeIsCenterAligned         (0u)
#define PWMA_DeadBandUsed                   (1u)
#define PWMA_DeadBand2_4                    (1u)

#if !defined(PWMA_PWMUDB_genblk8_stsreg__REMOVED)
    #define PWMA_UseStatus                  (1u)
#else
    #define PWMA_UseStatus                  (0u)
#endif /* !defined(PWMA_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(PWMA_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define PWMA_UseControl                 (1u)
#else
    #define PWMA_UseControl                 (0u)
#endif /* !defined(PWMA_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define PWMA_UseOneCompareMode              (0u)
#define PWMA_MinimumKillTime                (1u)
#define PWMA_EnableMode                     (0u)

#define PWMA_CompareMode1SW                 (0u)
#define PWMA_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define PWMA__B_PWM__DISABLED 0
#define PWMA__B_PWM__ASYNCHRONOUS 1
#define PWMA__B_PWM__SINGLECYCLE 2
#define PWMA__B_PWM__LATCHED 3
#define PWMA__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define PWMA__B_PWM__DBMDISABLED 0
#define PWMA__B_PWM__DBM_2_4_CLOCKS 1
#define PWMA__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define PWMA__B_PWM__ONE_OUTPUT 0
#define PWMA__B_PWM__TWO_OUTPUTS 1
#define PWMA__B_PWM__DUAL_EDGE 2
#define PWMA__B_PWM__CENTER_ALIGN 3
#define PWMA__B_PWM__DITHER 5
#define PWMA__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define PWMA__B_PWM__LESS_THAN 1
#define PWMA__B_PWM__LESS_THAN_OR_EQUAL 2
#define PWMA__B_PWM__GREATER_THAN 3
#define PWMA__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define PWMA__B_PWM__EQUAL 0
#define PWMA__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!PWMA_UsingFixedFunction)
        uint16 PWMUdb;               /* PWM Current Counter value  */
        #if(!PWMA_PWMModeIsCenterAligned)
            uint16 PWMPeriod;
        #endif /* (!PWMA_PWMModeIsCenterAligned) */
        #if (PWMA_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (PWMA_UseStatus) */

        /* Backup for Deadband parameters */
        #if(PWMA_DeadBandMode == PWMA__B_PWM__DBM_256_CLOCKS || \
            PWMA_DeadBandMode == PWMA__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(PWMA_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (PWMA_KillModeMinTime) */

        /* Backup control register */
        #if(PWMA_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (PWMA_UseControl) */

    #endif /* (!PWMA_UsingFixedFunction) */

}PWMA_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    PWMA_Start(void) ;
void    PWMA_Stop(void) ;

#if (PWMA_UseStatus || PWMA_UsingFixedFunction)
    void  PWMA_SetInterruptMode(uint8 interruptMode) ;
    uint8 PWMA_ReadStatusRegister(void) ;
#endif /* (PWMA_UseStatus || PWMA_UsingFixedFunction) */

#define PWMA_GetInterruptSource() PWMA_ReadStatusRegister()

#if (PWMA_UseControl)
    uint8 PWMA_ReadControlRegister(void) ;
    void  PWMA_WriteControlRegister(uint8 control)
          ;
#endif /* (PWMA_UseControl) */

#if (PWMA_UseOneCompareMode)
   #if (PWMA_CompareMode1SW)
       void    PWMA_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (PWMA_CompareMode1SW) */
#else
    #if (PWMA_CompareMode1SW)
        void    PWMA_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (PWMA_CompareMode1SW) */
    #if (PWMA_CompareMode2SW)
        void    PWMA_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (PWMA_CompareMode2SW) */
#endif /* (PWMA_UseOneCompareMode) */

#if (!PWMA_UsingFixedFunction)
    uint16   PWMA_ReadCounter(void) ;
    uint16 PWMA_ReadCapture(void) ;

    #if (PWMA_UseStatus)
            void PWMA_ClearFIFO(void) ;
    #endif /* (PWMA_UseStatus) */

    void    PWMA_WriteCounter(uint16 counter)
            ;
#endif /* (!PWMA_UsingFixedFunction) */

void    PWMA_WritePeriod(uint16 period)
        ;
uint16 PWMA_ReadPeriod(void) ;

#if (PWMA_UseOneCompareMode)
    void    PWMA_WriteCompare(uint16 compare)
            ;
    uint16 PWMA_ReadCompare(void) ;
#else
    void    PWMA_WriteCompare1(uint16 compare)
            ;
    uint16 PWMA_ReadCompare1(void) ;
    void    PWMA_WriteCompare2(uint16 compare)
            ;
    uint16 PWMA_ReadCompare2(void) ;
#endif /* (PWMA_UseOneCompareMode) */


#if (PWMA_DeadBandUsed)
    void    PWMA_WriteDeadTime(uint8 deadtime) ;
    uint8   PWMA_ReadDeadTime(void) ;
#endif /* (PWMA_DeadBandUsed) */

#if ( PWMA_KillModeMinTime)
    void PWMA_WriteKillTime(uint8 killtime) ;
    uint8 PWMA_ReadKillTime(void) ;
#endif /* ( PWMA_KillModeMinTime) */

void PWMA_Init(void) ;
void PWMA_Enable(void) ;
void PWMA_Sleep(void) ;
void PWMA_Wakeup(void) ;
void PWMA_SaveConfig(void) ;
void PWMA_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define PWMA_INIT_PERIOD_VALUE          (63998u)
#define PWMA_INIT_COMPARE_VALUE1        (4800u)
#define PWMA_INIT_COMPARE_VALUE2        (4800u)
#define PWMA_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    PWMA_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    PWMA_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    PWMA_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    PWMA_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define PWMA_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  PWMA_CTRL_CMPMODE2_SHIFT)
#define PWMA_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  PWMA_CTRL_CMPMODE1_SHIFT)
#define PWMA_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (PWMA_UsingFixedFunction)
   #define PWMA_PERIOD_LSB              (*(reg16 *) PWMA_PWMHW__PER0)
   #define PWMA_PERIOD_LSB_PTR          ( (reg16 *) PWMA_PWMHW__PER0)
   #define PWMA_COMPARE1_LSB            (*(reg16 *) PWMA_PWMHW__CNT_CMP0)
   #define PWMA_COMPARE1_LSB_PTR        ( (reg16 *) PWMA_PWMHW__CNT_CMP0)
   #define PWMA_COMPARE2_LSB            (0x00u)
   #define PWMA_COMPARE2_LSB_PTR        (0x00u)
   #define PWMA_COUNTER_LSB             (*(reg16 *) PWMA_PWMHW__CNT_CMP0)
   #define PWMA_COUNTER_LSB_PTR         ( (reg16 *) PWMA_PWMHW__CNT_CMP0)
   #define PWMA_CAPTURE_LSB             (*(reg16 *) PWMA_PWMHW__CAP0)
   #define PWMA_CAPTURE_LSB_PTR         ( (reg16 *) PWMA_PWMHW__CAP0)
   #define PWMA_RT1                     (*(reg8 *)  PWMA_PWMHW__RT1)
   #define PWMA_RT1_PTR                 ( (reg8 *)  PWMA_PWMHW__RT1)

#else
   #if (PWMA_Resolution == 8u) /* 8bit - PWM */

       #if(PWMA_PWMModeIsCenterAligned)
           #define PWMA_PERIOD_LSB      (*(reg8 *)  PWMA_PWMUDB_sP16_pwmdp_u0__D1_REG)
           #define PWMA_PERIOD_LSB_PTR  ((reg8 *)   PWMA_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #else
           #define PWMA_PERIOD_LSB      (*(reg8 *)  PWMA_PWMUDB_sP16_pwmdp_u0__F0_REG)
           #define PWMA_PERIOD_LSB_PTR  ((reg8 *)   PWMA_PWMUDB_sP16_pwmdp_u0__F0_REG)
       #endif /* (PWMA_PWMModeIsCenterAligned) */

       #define PWMA_COMPARE1_LSB        (*(reg8 *)  PWMA_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define PWMA_COMPARE1_LSB_PTR    ((reg8 *)   PWMA_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define PWMA_COMPARE2_LSB        (*(reg8 *)  PWMA_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define PWMA_COMPARE2_LSB_PTR    ((reg8 *)   PWMA_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define PWMA_COUNTERCAP_LSB      (*(reg8 *)  PWMA_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define PWMA_COUNTERCAP_LSB_PTR  ((reg8 *)   PWMA_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define PWMA_COUNTER_LSB         (*(reg8 *)  PWMA_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define PWMA_COUNTER_LSB_PTR     ((reg8 *)   PWMA_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define PWMA_CAPTURE_LSB         (*(reg8 *)  PWMA_PWMUDB_sP16_pwmdp_u0__F1_REG)
       #define PWMA_CAPTURE_LSB_PTR     ((reg8 *)   PWMA_PWMUDB_sP16_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(PWMA_PWMModeIsCenterAligned)
               #define PWMA_PERIOD_LSB      (*(reg16 *) PWMA_PWMUDB_sP16_pwmdp_u0__D1_REG)
               #define PWMA_PERIOD_LSB_PTR  ((reg16 *)  PWMA_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #else
               #define PWMA_PERIOD_LSB      (*(reg16 *) PWMA_PWMUDB_sP16_pwmdp_u0__F0_REG)
               #define PWMA_PERIOD_LSB_PTR  ((reg16 *)  PWMA_PWMUDB_sP16_pwmdp_u0__F0_REG)
            #endif /* (PWMA_PWMModeIsCenterAligned) */

            #define PWMA_COMPARE1_LSB       (*(reg16 *) PWMA_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define PWMA_COMPARE1_LSB_PTR   ((reg16 *)  PWMA_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define PWMA_COMPARE2_LSB       (*(reg16 *) PWMA_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define PWMA_COMPARE2_LSB_PTR   ((reg16 *)  PWMA_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define PWMA_COUNTERCAP_LSB     (*(reg16 *) PWMA_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define PWMA_COUNTERCAP_LSB_PTR ((reg16 *)  PWMA_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define PWMA_COUNTER_LSB        (*(reg16 *) PWMA_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define PWMA_COUNTER_LSB_PTR    ((reg16 *)  PWMA_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define PWMA_CAPTURE_LSB        (*(reg16 *) PWMA_PWMUDB_sP16_pwmdp_u0__F1_REG)
            #define PWMA_CAPTURE_LSB_PTR    ((reg16 *)  PWMA_PWMUDB_sP16_pwmdp_u0__F1_REG)
        #else
            #if(PWMA_PWMModeIsCenterAligned)
               #define PWMA_PERIOD_LSB      (*(reg16 *) PWMA_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
               #define PWMA_PERIOD_LSB_PTR  ((reg16 *)  PWMA_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #else
               #define PWMA_PERIOD_LSB      (*(reg16 *) PWMA_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
               #define PWMA_PERIOD_LSB_PTR  ((reg16 *)  PWMA_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
            #endif /* (PWMA_PWMModeIsCenterAligned) */

            #define PWMA_COMPARE1_LSB       (*(reg16 *) PWMA_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define PWMA_COMPARE1_LSB_PTR   ((reg16 *)  PWMA_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define PWMA_COMPARE2_LSB       (*(reg16 *) PWMA_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define PWMA_COMPARE2_LSB_PTR   ((reg16 *)  PWMA_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define PWMA_COUNTERCAP_LSB     (*(reg16 *) PWMA_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define PWMA_COUNTERCAP_LSB_PTR ((reg16 *)  PWMA_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define PWMA_COUNTER_LSB        (*(reg16 *) PWMA_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define PWMA_COUNTER_LSB_PTR    ((reg16 *)  PWMA_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define PWMA_CAPTURE_LSB        (*(reg16 *) PWMA_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
            #define PWMA_CAPTURE_LSB_PTR    ((reg16 *)  PWMA_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define PWMA_AUX_CONTROLDP1          (*(reg8 *)  PWMA_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)
       #define PWMA_AUX_CONTROLDP1_PTR      ((reg8 *)   PWMA_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (PWMA_Resolution == 8) */

   #define PWMA_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  PWMA_PWMUDB_sP16_pwmdp_u0__A1_REG)
   #define PWMA_AUX_CONTROLDP0          (*(reg8 *)  PWMA_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)
   #define PWMA_AUX_CONTROLDP0_PTR      ((reg8 *)   PWMA_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (PWMA_UsingFixedFunction) */

#if(PWMA_KillModeMinTime )
    #define PWMA_KILLMODEMINTIME        (*(reg8 *)  PWMA_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define PWMA_KILLMODEMINTIME_PTR    ((reg8 *)   PWMA_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (PWMA_KillModeMinTime ) */

#if(PWMA_DeadBandMode == PWMA__B_PWM__DBM_256_CLOCKS)
    #define PWMA_DEADBAND_COUNT         (*(reg8 *)  PWMA_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define PWMA_DEADBAND_COUNT_PTR     ((reg8 *)   PWMA_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define PWMA_DEADBAND_LSB_PTR       ((reg8 *)   PWMA_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define PWMA_DEADBAND_LSB           (*(reg8 *)  PWMA_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(PWMA_DeadBandMode == PWMA__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (PWMA_UsingFixedFunction)
        #define PWMA_DEADBAND_COUNT         (*(reg8 *)  PWMA_PWMHW__CFG0)
        #define PWMA_DEADBAND_COUNT_PTR     ((reg8 *)   PWMA_PWMHW__CFG0)
        #define PWMA_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << PWMA_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define PWMA_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define PWMA_DEADBAND_COUNT         (*(reg8 *)  PWMA_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define PWMA_DEADBAND_COUNT_PTR     ((reg8 *)   PWMA_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define PWMA_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << PWMA_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define PWMA_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (PWMA_UsingFixedFunction) */
#endif /* (PWMA_DeadBandMode == PWMA__B_PWM__DBM_256_CLOCKS) */



#if (PWMA_UsingFixedFunction)
    #define PWMA_STATUS                 (*(reg8 *) PWMA_PWMHW__SR0)
    #define PWMA_STATUS_PTR             ((reg8 *) PWMA_PWMHW__SR0)
    #define PWMA_STATUS_MASK            (*(reg8 *) PWMA_PWMHW__SR0)
    #define PWMA_STATUS_MASK_PTR        ((reg8 *) PWMA_PWMHW__SR0)
    #define PWMA_CONTROL                (*(reg8 *) PWMA_PWMHW__CFG0)
    #define PWMA_CONTROL_PTR            ((reg8 *) PWMA_PWMHW__CFG0)
    #define PWMA_CONTROL2               (*(reg8 *) PWMA_PWMHW__CFG1)
    #define PWMA_CONTROL3               (*(reg8 *) PWMA_PWMHW__CFG2)
    #define PWMA_GLOBAL_ENABLE          (*(reg8 *) PWMA_PWMHW__PM_ACT_CFG)
    #define PWMA_GLOBAL_ENABLE_PTR      ( (reg8 *) PWMA_PWMHW__PM_ACT_CFG)
    #define PWMA_GLOBAL_STBY_ENABLE     (*(reg8 *) PWMA_PWMHW__PM_STBY_CFG)
    #define PWMA_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) PWMA_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define PWMA_BLOCK_EN_MASK          (PWMA_PWMHW__PM_ACT_MSK)
    #define PWMA_BLOCK_STBY_EN_MASK     (PWMA_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define PWMA_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define PWMA_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define PWMA_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define PWMA_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define PWMA_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define PWMA_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define PWMA_CTRL_ENABLE            (uint8)((uint8)0x01u << PWMA_CTRL_ENABLE_SHIFT)
    #define PWMA_CTRL_RESET             (uint8)((uint8)0x01u << PWMA_CTRL_RESET_SHIFT)
    #define PWMA_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << PWMA_CTRL_CMPMODE2_SHIFT)
    #define PWMA_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << PWMA_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define PWMA_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define PWMA_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << PWMA_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define PWMA_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define PWMA_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define PWMA_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define PWMA_STATUS_TC_INT_EN_MASK_SHIFT            (PWMA_STATUS_TC_SHIFT - 4u)
    #define PWMA_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define PWMA_STATUS_CMP1_INT_EN_MASK_SHIFT          (PWMA_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define PWMA_STATUS_TC              (uint8)((uint8)0x01u << PWMA_STATUS_TC_SHIFT)
    #define PWMA_STATUS_CMP1            (uint8)((uint8)0x01u << PWMA_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define PWMA_STATUS_TC_INT_EN_MASK              (uint8)((uint8)PWMA_STATUS_TC >> 4u)
    #define PWMA_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)PWMA_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define PWMA_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define PWMA_RT1_MASK              (uint8)((uint8)0x03u << PWMA_RT1_SHIFT)
    #define PWMA_SYNC                  (uint8)((uint8)0x03u << PWMA_RT1_SHIFT)
    #define PWMA_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define PWMA_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << PWMA_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define PWMA_SYNCDSI_EN            (uint8)((uint8)0x0Fu << PWMA_SYNCDSI_SHIFT)


#else
    #define PWMA_STATUS                (*(reg8 *)   PWMA_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define PWMA_STATUS_PTR            ((reg8 *)    PWMA_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define PWMA_STATUS_MASK           (*(reg8 *)   PWMA_PWMUDB_genblk8_stsreg__MASK_REG)
    #define PWMA_STATUS_MASK_PTR       ((reg8 *)    PWMA_PWMUDB_genblk8_stsreg__MASK_REG)
    #define PWMA_STATUS_AUX_CTRL       (*(reg8 *)   PWMA_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define PWMA_CONTROL               (*(reg8 *)   PWMA_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define PWMA_CONTROL_PTR           ((reg8 *)    PWMA_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define PWMA_CTRL_ENABLE_SHIFT      (0x07u)
    #define PWMA_CTRL_RESET_SHIFT       (0x06u)
    #define PWMA_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define PWMA_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define PWMA_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define PWMA_CTRL_ENABLE            (uint8)((uint8)0x01u << PWMA_CTRL_ENABLE_SHIFT)
    #define PWMA_CTRL_RESET             (uint8)((uint8)0x01u << PWMA_CTRL_RESET_SHIFT)
    #define PWMA_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << PWMA_CTRL_CMPMODE2_SHIFT)
    #define PWMA_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << PWMA_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define PWMA_STATUS_KILL_SHIFT          (0x05u)
    #define PWMA_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define PWMA_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define PWMA_STATUS_TC_SHIFT            (0x02u)
    #define PWMA_STATUS_CMP2_SHIFT          (0x01u)
    #define PWMA_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define PWMA_STATUS_KILL_INT_EN_MASK_SHIFT          (PWMA_STATUS_KILL_SHIFT)
    #define PWMA_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (PWMA_STATUS_FIFONEMPTY_SHIFT)
    #define PWMA_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (PWMA_STATUS_FIFOFULL_SHIFT)
    #define PWMA_STATUS_TC_INT_EN_MASK_SHIFT            (PWMA_STATUS_TC_SHIFT)
    #define PWMA_STATUS_CMP2_INT_EN_MASK_SHIFT          (PWMA_STATUS_CMP2_SHIFT)
    #define PWMA_STATUS_CMP1_INT_EN_MASK_SHIFT          (PWMA_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define PWMA_STATUS_KILL            (uint8)((uint8)0x00u << PWMA_STATUS_KILL_SHIFT )
    #define PWMA_STATUS_FIFOFULL        (uint8)((uint8)0x01u << PWMA_STATUS_FIFOFULL_SHIFT)
    #define PWMA_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << PWMA_STATUS_FIFONEMPTY_SHIFT)
    #define PWMA_STATUS_TC              (uint8)((uint8)0x01u << PWMA_STATUS_TC_SHIFT)
    #define PWMA_STATUS_CMP2            (uint8)((uint8)0x01u << PWMA_STATUS_CMP2_SHIFT)
    #define PWMA_STATUS_CMP1            (uint8)((uint8)0x01u << PWMA_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define PWMA_STATUS_KILL_INT_EN_MASK            (PWMA_STATUS_KILL)
    #define PWMA_STATUS_FIFOFULL_INT_EN_MASK        (PWMA_STATUS_FIFOFULL)
    #define PWMA_STATUS_FIFONEMPTY_INT_EN_MASK      (PWMA_STATUS_FIFONEMPTY)
    #define PWMA_STATUS_TC_INT_EN_MASK              (PWMA_STATUS_TC)
    #define PWMA_STATUS_CMP2_INT_EN_MASK            (PWMA_STATUS_CMP2)
    #define PWMA_STATUS_CMP1_INT_EN_MASK            (PWMA_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define PWMA_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define PWMA_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define PWMA_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define PWMA_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define PWMA_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* PWMA_UsingFixedFunction */

#endif  /* CY_PWM_PWMA_H */


/* [] END OF FILE */
