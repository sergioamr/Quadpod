/*******************************************************************************
* File Name: PWMSpd.h
* Version 2.0
*
* Description:
*  This file provides constants and parameter values for the PWMSpd
*  component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_TCPWM_PWMSpd_H)
#define CY_TCPWM_PWMSpd_H


#include "CyLib.h"
#include "cytypes.h"
#include "cyfitter.h"


/*******************************************************************************
* Internal Type defines
*******************************************************************************/

/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} PWMSpd_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  PWMSpd_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define PWMSpd_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define PWMSpd_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define PWMSpd_CONFIG                         (7lu)

/* Quad Mode */
/* Parameters */
#define PWMSpd_QUAD_ENCODING_MODES            (0lu)

/* Signal modes */
#define PWMSpd_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define PWMSpd_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define PWMSpd_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define PWMSpd_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define PWMSpd_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define PWMSpd_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define PWMSpd_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define PWMSpd_TC_RUN_MODE                    (0lu)
#define PWMSpd_TC_COUNTER_MODE                (0lu)
#define PWMSpd_TC_COMP_CAP_MODE               (2lu)
#define PWMSpd_TC_PRESCALER                   (0lu)

/* Signal modes */
#define PWMSpd_TC_RELOAD_SIGNAL_MODE          (0lu)
#define PWMSpd_TC_COUNT_SIGNAL_MODE           (3lu)
#define PWMSpd_TC_START_SIGNAL_MODE           (0lu)
#define PWMSpd_TC_STOP_SIGNAL_MODE            (0lu)
#define PWMSpd_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define PWMSpd_TC_RELOAD_SIGNAL_PRESENT       (0lu)
#define PWMSpd_TC_COUNT_SIGNAL_PRESENT        (0lu)
#define PWMSpd_TC_START_SIGNAL_PRESENT        (0lu)
#define PWMSpd_TC_STOP_SIGNAL_PRESENT         (0lu)
#define PWMSpd_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define PWMSpd_TC_INTERRUPT_MASK              (1lu)

/* PWM Mode */
/* Parameters */
#define PWMSpd_PWM_KILL_EVENT                 (0lu)
#define PWMSpd_PWM_STOP_EVENT                 (0lu)
#define PWMSpd_PWM_MODE                       (4lu)
#define PWMSpd_PWM_OUT_N_INVERT               (0lu)
#define PWMSpd_PWM_OUT_INVERT                 (0lu)
#define PWMSpd_PWM_ALIGN                      (0lu)
#define PWMSpd_PWM_RUN_MODE                   (0lu)
#define PWMSpd_PWM_DEAD_TIME_CYCLE            (0lu)
#define PWMSpd_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define PWMSpd_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define PWMSpd_PWM_COUNT_SIGNAL_MODE          (3lu)
#define PWMSpd_PWM_START_SIGNAL_MODE          (0lu)
#define PWMSpd_PWM_STOP_SIGNAL_MODE           (0lu)
#define PWMSpd_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define PWMSpd_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define PWMSpd_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define PWMSpd_PWM_START_SIGNAL_PRESENT       (0lu)
#define PWMSpd_PWM_STOP_SIGNAL_PRESENT        (0lu)
#define PWMSpd_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define PWMSpd_PWM_INTERRUPT_MASK             (0lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define PWMSpd_TC_PERIOD_VALUE                (65535lu)
#define PWMSpd_TC_COMPARE_VALUE               (65535lu)
#define PWMSpd_TC_COMPARE_BUF_VALUE           (65535lu)
#define PWMSpd_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define PWMSpd_PWM_PERIOD_VALUE               (255lu)
#define PWMSpd_PWM_PERIOD_BUF_VALUE           (65535lu)
#define PWMSpd_PWM_PERIOD_SWAP                (0lu)
#define PWMSpd_PWM_COMPARE_VALUE              (0lu)
#define PWMSpd_PWM_COMPARE_BUF_VALUE          (65535lu)
#define PWMSpd_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define PWMSpd__LEFT 0
#define PWMSpd__RIGHT 1
#define PWMSpd__CENTER 2
#define PWMSpd__ASYMMETRIC 3

#define PWMSpd__X1 0
#define PWMSpd__X2 1
#define PWMSpd__X4 2

#define PWMSpd__PWM 4
#define PWMSpd__PWM_DT 5
#define PWMSpd__PWM_PR 6

#define PWMSpd__INVERSE 1
#define PWMSpd__DIRECT 0

#define PWMSpd__CAPTURE 2
#define PWMSpd__COMPARE 0

#define PWMSpd__TRIG_LEVEL 3
#define PWMSpd__TRIG_RISING 0
#define PWMSpd__TRIG_FALLING 1
#define PWMSpd__TRIG_BOTH 2

#define PWMSpd__INTR_MASK_TC 1
#define PWMSpd__INTR_MASK_CC_MATCH 2
#define PWMSpd__INTR_MASK_NONE 0
#define PWMSpd__INTR_MASK_TC_CC 3

#define PWMSpd__UNCONFIG 8
#define PWMSpd__TIMER 1
#define PWMSpd__QUAD 3
#define PWMSpd__PWM_SEL 7

#define PWMSpd__COUNT_UP 0
#define PWMSpd__COUNT_DOWN 1
#define PWMSpd__COUNT_UPDOWN0 2
#define PWMSpd__COUNT_UPDOWN1 3


/* Prescaler */
#define PWMSpd_PRESCALE_DIVBY1                ((uint32)(0u << PWMSpd_PRESCALER_SHIFT))
#define PWMSpd_PRESCALE_DIVBY2                ((uint32)(1u << PWMSpd_PRESCALER_SHIFT))
#define PWMSpd_PRESCALE_DIVBY4                ((uint32)(2u << PWMSpd_PRESCALER_SHIFT))
#define PWMSpd_PRESCALE_DIVBY8                ((uint32)(3u << PWMSpd_PRESCALER_SHIFT))
#define PWMSpd_PRESCALE_DIVBY16               ((uint32)(4u << PWMSpd_PRESCALER_SHIFT))
#define PWMSpd_PRESCALE_DIVBY32               ((uint32)(5u << PWMSpd_PRESCALER_SHIFT))
#define PWMSpd_PRESCALE_DIVBY64               ((uint32)(6u << PWMSpd_PRESCALER_SHIFT))
#define PWMSpd_PRESCALE_DIVBY128              ((uint32)(7u << PWMSpd_PRESCALER_SHIFT))

/* TCPWM set modes */
#define PWMSpd_MODE_TIMER_COMPARE             ((uint32)(PWMSpd__COMPARE         <<  \
                                                                  PWMSpd_MODE_SHIFT))
#define PWMSpd_MODE_TIMER_CAPTURE             ((uint32)(PWMSpd__CAPTURE         <<  \
                                                                  PWMSpd_MODE_SHIFT))
#define PWMSpd_MODE_QUAD                      ((uint32)(PWMSpd__QUAD            <<  \
                                                                  PWMSpd_MODE_SHIFT))
#define PWMSpd_MODE_PWM                       ((uint32)(PWMSpd__PWM             <<  \
                                                                  PWMSpd_MODE_SHIFT))
#define PWMSpd_MODE_PWM_DT                    ((uint32)(PWMSpd__PWM_DT          <<  \
                                                                  PWMSpd_MODE_SHIFT))
#define PWMSpd_MODE_PWM_PR                    ((uint32)(PWMSpd__PWM_PR          <<  \
                                                                  PWMSpd_MODE_SHIFT))

/* Quad Modes */
#define PWMSpd_MODE_X1                        ((uint32)(PWMSpd__X1              <<  \
                                                                  PWMSpd_QUAD_MODE_SHIFT))
#define PWMSpd_MODE_X2                        ((uint32)(PWMSpd__X2              <<  \
                                                                  PWMSpd_QUAD_MODE_SHIFT))
#define PWMSpd_MODE_X4                        ((uint32)(PWMSpd__X4              <<  \
                                                                  PWMSpd_QUAD_MODE_SHIFT))

/* Counter modes */
#define PWMSpd_COUNT_UP                       ((uint32)(PWMSpd__COUNT_UP        <<  \
                                                                  PWMSpd_UPDOWN_SHIFT))
#define PWMSpd_COUNT_DOWN                     ((uint32)(PWMSpd__COUNT_DOWN      <<  \
                                                                  PWMSpd_UPDOWN_SHIFT))
#define PWMSpd_COUNT_UPDOWN0                  ((uint32)(PWMSpd__COUNT_UPDOWN0   <<  \
                                                                  PWMSpd_UPDOWN_SHIFT))
#define PWMSpd_COUNT_UPDOWN1                  ((uint32)(PWMSpd__COUNT_UPDOWN1   <<  \
                                                                  PWMSpd_UPDOWN_SHIFT))

/* PWM output invert */
#define PWMSpd_INVERT_LINE                    ((uint32)(PWMSpd__INVERSE         <<  \
                                                                  PWMSpd_INV_OUT_SHIFT))
#define PWMSpd_INVERT_LINE_N                  ((uint32)(PWMSpd__INVERSE         <<  \
                                                                  PWMSpd_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define PWMSpd_TRIG_RISING                    ((uint32)PWMSpd__TRIG_RISING)
#define PWMSpd_TRIG_FALLING                   ((uint32)PWMSpd__TRIG_FALLING)
#define PWMSpd_TRIG_BOTH                      ((uint32)PWMSpd__TRIG_BOTH)
#define PWMSpd_TRIG_LEVEL                     ((uint32)PWMSpd__TRIG_LEVEL)

/* Interrupt mask */
#define PWMSpd_INTR_MASK_TC                   ((uint32)PWMSpd__INTR_MASK_TC)
#define PWMSpd_INTR_MASK_CC_MATCH             ((uint32)PWMSpd__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define PWMSpd_CC_MATCH_SET                   (0x00u)
#define PWMSpd_CC_MATCH_CLEAR                 (0x01u)
#define PWMSpd_CC_MATCH_INVERT                (0x02u)
#define PWMSpd_CC_MATCH_NO_CHANGE             (0x03u)
#define PWMSpd_OVERLOW_SET                    (0x00u)
#define PWMSpd_OVERLOW_CLEAR                  (0x04u)
#define PWMSpd_OVERLOW_INVERT                 (0x08u)
#define PWMSpd_OVERLOW_NO_CHANGE              (0x0Cu)
#define PWMSpd_UNDERFLOW_SET                  (0x00u)
#define PWMSpd_UNDERFLOW_CLEAR                (0x10u)
#define PWMSpd_UNDERFLOW_INVERT               (0x20u)
#define PWMSpd_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define PWMSpd_PWM_MODE_LEFT                  (PWMSpd_CC_MATCH_CLEAR        |   \
                                                         PWMSpd_OVERLOW_SET           |   \
                                                         PWMSpd_UNDERFLOW_NO_CHANGE)
#define PWMSpd_PWM_MODE_RIGHT                 (PWMSpd_CC_MATCH_SET          |   \
                                                         PWMSpd_OVERLOW_NO_CHANGE     |   \
                                                         PWMSpd_UNDERFLOW_CLEAR)
#define PWMSpd_PWM_MODE_ASYM                  (PWMSpd_CC_MATCH_INVERT       |   \
                                                         PWMSpd_OVERLOW_SET           |   \
                                                         PWMSpd_UNDERFLOW_CLEAR)

#if (PWMSpd_CY_TCPWM_V2)
    #if(PWMSpd_CY_TCPWM_4000)
        #define PWMSpd_PWM_MODE_CENTER                (PWMSpd_CC_MATCH_INVERT       |   \
                                                                 PWMSpd_OVERLOW_NO_CHANGE     |   \
                                                                 PWMSpd_UNDERFLOW_CLEAR)
    #else
        #define PWMSpd_PWM_MODE_CENTER                (PWMSpd_CC_MATCH_INVERT       |   \
                                                                 PWMSpd_OVERLOW_SET           |   \
                                                                 PWMSpd_UNDERFLOW_CLEAR)
    #endif /* (PWMSpd_CY_TCPWM_4000) */
#else
    #define PWMSpd_PWM_MODE_CENTER                (PWMSpd_CC_MATCH_INVERT       |   \
                                                             PWMSpd_OVERLOW_NO_CHANGE     |   \
                                                             PWMSpd_UNDERFLOW_CLEAR)
#endif /* (PWMSpd_CY_TCPWM_NEW) */

/* Command operations without condition */
#define PWMSpd_CMD_CAPTURE                    (0u)
#define PWMSpd_CMD_RELOAD                     (8u)
#define PWMSpd_CMD_STOP                       (16u)
#define PWMSpd_CMD_START                      (24u)

/* Status */
#define PWMSpd_STATUS_DOWN                    (1u)
#define PWMSpd_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   PWMSpd_Init(void);
void   PWMSpd_Enable(void);
void   PWMSpd_Start(void);
void   PWMSpd_Stop(void);

void   PWMSpd_SetMode(uint32 mode);
void   PWMSpd_SetCounterMode(uint32 counterMode);
void   PWMSpd_SetPWMMode(uint32 modeMask);
void   PWMSpd_SetQDMode(uint32 qdMode);

void   PWMSpd_SetPrescaler(uint32 prescaler);
void   PWMSpd_TriggerCommand(uint32 mask, uint32 command);
void   PWMSpd_SetOneShot(uint32 oneShotEnable);
uint32 PWMSpd_ReadStatus(void);

void   PWMSpd_SetPWMSyncKill(uint32 syncKillEnable);
void   PWMSpd_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   PWMSpd_SetPWMDeadTime(uint32 deadTime);
void   PWMSpd_SetPWMInvert(uint32 mask);

void   PWMSpd_SetInterruptMode(uint32 interruptMask);
uint32 PWMSpd_GetInterruptSourceMasked(void);
uint32 PWMSpd_GetInterruptSource(void);
void   PWMSpd_ClearInterrupt(uint32 interruptMask);
void   PWMSpd_SetInterrupt(uint32 interruptMask);

void   PWMSpd_WriteCounter(uint32 count);
uint32 PWMSpd_ReadCounter(void);

uint32 PWMSpd_ReadCapture(void);
uint32 PWMSpd_ReadCaptureBuf(void);

void   PWMSpd_WritePeriod(uint32 period);
uint32 PWMSpd_ReadPeriod(void);
void   PWMSpd_WritePeriodBuf(uint32 periodBuf);
uint32 PWMSpd_ReadPeriodBuf(void);

void   PWMSpd_WriteCompare(uint32 compare);
uint32 PWMSpd_ReadCompare(void);
void   PWMSpd_WriteCompareBuf(uint32 compareBuf);
uint32 PWMSpd_ReadCompareBuf(void);

void   PWMSpd_SetPeriodSwap(uint32 swapEnable);
void   PWMSpd_SetCompareSwap(uint32 swapEnable);

void   PWMSpd_SetCaptureMode(uint32 triggerMode);
void   PWMSpd_SetReloadMode(uint32 triggerMode);
void   PWMSpd_SetStartMode(uint32 triggerMode);
void   PWMSpd_SetStopMode(uint32 triggerMode);
void   PWMSpd_SetCountMode(uint32 triggerMode);

void   PWMSpd_SaveConfig(void);
void   PWMSpd_RestoreConfig(void);
void   PWMSpd_Sleep(void);
void   PWMSpd_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define PWMSpd_BLOCK_CONTROL_REG              (*(reg32 *) PWMSpd_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define PWMSpd_BLOCK_CONTROL_PTR              ( (reg32 *) PWMSpd_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define PWMSpd_COMMAND_REG                    (*(reg32 *) PWMSpd_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define PWMSpd_COMMAND_PTR                    ( (reg32 *) PWMSpd_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define PWMSpd_INTRRUPT_CAUSE_REG             (*(reg32 *) PWMSpd_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define PWMSpd_INTRRUPT_CAUSE_PTR             ( (reg32 *) PWMSpd_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define PWMSpd_CONTROL_REG                    (*(reg32 *) PWMSpd_cy_m0s8_tcpwm_1__CTRL )
#define PWMSpd_CONTROL_PTR                    ( (reg32 *) PWMSpd_cy_m0s8_tcpwm_1__CTRL )
#define PWMSpd_STATUS_REG                     (*(reg32 *) PWMSpd_cy_m0s8_tcpwm_1__STATUS )
#define PWMSpd_STATUS_PTR                     ( (reg32 *) PWMSpd_cy_m0s8_tcpwm_1__STATUS )
#define PWMSpd_COUNTER_REG                    (*(reg32 *) PWMSpd_cy_m0s8_tcpwm_1__COUNTER )
#define PWMSpd_COUNTER_PTR                    ( (reg32 *) PWMSpd_cy_m0s8_tcpwm_1__COUNTER )
#define PWMSpd_COMP_CAP_REG                   (*(reg32 *) PWMSpd_cy_m0s8_tcpwm_1__CC )
#define PWMSpd_COMP_CAP_PTR                   ( (reg32 *) PWMSpd_cy_m0s8_tcpwm_1__CC )
#define PWMSpd_COMP_CAP_BUF_REG               (*(reg32 *) PWMSpd_cy_m0s8_tcpwm_1__CC_BUFF )
#define PWMSpd_COMP_CAP_BUF_PTR               ( (reg32 *) PWMSpd_cy_m0s8_tcpwm_1__CC_BUFF )
#define PWMSpd_PERIOD_REG                     (*(reg32 *) PWMSpd_cy_m0s8_tcpwm_1__PERIOD )
#define PWMSpd_PERIOD_PTR                     ( (reg32 *) PWMSpd_cy_m0s8_tcpwm_1__PERIOD )
#define PWMSpd_PERIOD_BUF_REG                 (*(reg32 *) PWMSpd_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define PWMSpd_PERIOD_BUF_PTR                 ( (reg32 *) PWMSpd_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define PWMSpd_TRIG_CONTROL0_REG              (*(reg32 *) PWMSpd_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define PWMSpd_TRIG_CONTROL0_PTR              ( (reg32 *) PWMSpd_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define PWMSpd_TRIG_CONTROL1_REG              (*(reg32 *) PWMSpd_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define PWMSpd_TRIG_CONTROL1_PTR              ( (reg32 *) PWMSpd_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define PWMSpd_TRIG_CONTROL2_REG              (*(reg32 *) PWMSpd_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define PWMSpd_TRIG_CONTROL2_PTR              ( (reg32 *) PWMSpd_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define PWMSpd_INTERRUPT_REQ_REG              (*(reg32 *) PWMSpd_cy_m0s8_tcpwm_1__INTR )
#define PWMSpd_INTERRUPT_REQ_PTR              ( (reg32 *) PWMSpd_cy_m0s8_tcpwm_1__INTR )
#define PWMSpd_INTERRUPT_SET_REG              (*(reg32 *) PWMSpd_cy_m0s8_tcpwm_1__INTR_SET )
#define PWMSpd_INTERRUPT_SET_PTR              ( (reg32 *) PWMSpd_cy_m0s8_tcpwm_1__INTR_SET )
#define PWMSpd_INTERRUPT_MASK_REG             (*(reg32 *) PWMSpd_cy_m0s8_tcpwm_1__INTR_MASK )
#define PWMSpd_INTERRUPT_MASK_PTR             ( (reg32 *) PWMSpd_cy_m0s8_tcpwm_1__INTR_MASK )
#define PWMSpd_INTERRUPT_MASKED_REG           (*(reg32 *) PWMSpd_cy_m0s8_tcpwm_1__INTR_MASKED )
#define PWMSpd_INTERRUPT_MASKED_PTR           ( (reg32 *) PWMSpd_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define PWMSpd_MASK                           ((uint32)PWMSpd_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define PWMSpd_RELOAD_CC_SHIFT                (0u)
#define PWMSpd_RELOAD_PERIOD_SHIFT            (1u)
#define PWMSpd_PWM_SYNC_KILL_SHIFT            (2u)
#define PWMSpd_PWM_STOP_KILL_SHIFT            (3u)
#define PWMSpd_PRESCALER_SHIFT                (8u)
#define PWMSpd_UPDOWN_SHIFT                   (16u)
#define PWMSpd_ONESHOT_SHIFT                  (18u)
#define PWMSpd_QUAD_MODE_SHIFT                (20u)
#define PWMSpd_INV_OUT_SHIFT                  (20u)
#define PWMSpd_INV_COMPL_OUT_SHIFT            (21u)
#define PWMSpd_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define PWMSpd_RELOAD_CC_MASK                 ((uint32)(PWMSpd_1BIT_MASK        <<  \
                                                                            PWMSpd_RELOAD_CC_SHIFT))
#define PWMSpd_RELOAD_PERIOD_MASK             ((uint32)(PWMSpd_1BIT_MASK        <<  \
                                                                            PWMSpd_RELOAD_PERIOD_SHIFT))
#define PWMSpd_PWM_SYNC_KILL_MASK             ((uint32)(PWMSpd_1BIT_MASK        <<  \
                                                                            PWMSpd_PWM_SYNC_KILL_SHIFT))
#define PWMSpd_PWM_STOP_KILL_MASK             ((uint32)(PWMSpd_1BIT_MASK        <<  \
                                                                            PWMSpd_PWM_STOP_KILL_SHIFT))
#define PWMSpd_PRESCALER_MASK                 ((uint32)(PWMSpd_8BIT_MASK        <<  \
                                                                            PWMSpd_PRESCALER_SHIFT))
#define PWMSpd_UPDOWN_MASK                    ((uint32)(PWMSpd_2BIT_MASK        <<  \
                                                                            PWMSpd_UPDOWN_SHIFT))
#define PWMSpd_ONESHOT_MASK                   ((uint32)(PWMSpd_1BIT_MASK        <<  \
                                                                            PWMSpd_ONESHOT_SHIFT))
#define PWMSpd_QUAD_MODE_MASK                 ((uint32)(PWMSpd_3BIT_MASK        <<  \
                                                                            PWMSpd_QUAD_MODE_SHIFT))
#define PWMSpd_INV_OUT_MASK                   ((uint32)(PWMSpd_2BIT_MASK        <<  \
                                                                            PWMSpd_INV_OUT_SHIFT))
#define PWMSpd_MODE_MASK                      ((uint32)(PWMSpd_3BIT_MASK        <<  \
                                                                            PWMSpd_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define PWMSpd_CAPTURE_SHIFT                  (0u)
#define PWMSpd_COUNT_SHIFT                    (2u)
#define PWMSpd_RELOAD_SHIFT                   (4u)
#define PWMSpd_STOP_SHIFT                     (6u)
#define PWMSpd_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define PWMSpd_CAPTURE_MASK                   ((uint32)(PWMSpd_2BIT_MASK        <<  \
                                                                  PWMSpd_CAPTURE_SHIFT))
#define PWMSpd_COUNT_MASK                     ((uint32)(PWMSpd_2BIT_MASK        <<  \
                                                                  PWMSpd_COUNT_SHIFT))
#define PWMSpd_RELOAD_MASK                    ((uint32)(PWMSpd_2BIT_MASK        <<  \
                                                                  PWMSpd_RELOAD_SHIFT))
#define PWMSpd_STOP_MASK                      ((uint32)(PWMSpd_2BIT_MASK        <<  \
                                                                  PWMSpd_STOP_SHIFT))
#define PWMSpd_START_MASK                     ((uint32)(PWMSpd_2BIT_MASK        <<  \
                                                                  PWMSpd_START_SHIFT))

/* MASK */
#define PWMSpd_1BIT_MASK                      ((uint32)0x01u)
#define PWMSpd_2BIT_MASK                      ((uint32)0x03u)
#define PWMSpd_3BIT_MASK                      ((uint32)0x07u)
#define PWMSpd_6BIT_MASK                      ((uint32)0x3Fu)
#define PWMSpd_8BIT_MASK                      ((uint32)0xFFu)
#define PWMSpd_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define PWMSpd_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define PWMSpd_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(PWMSpd_QUAD_ENCODING_MODES     << PWMSpd_QUAD_MODE_SHIFT))       |\
         ((uint32)(PWMSpd_CONFIG                  << PWMSpd_MODE_SHIFT)))

#define PWMSpd_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(PWMSpd_PWM_STOP_EVENT          << PWMSpd_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(PWMSpd_PWM_OUT_INVERT          << PWMSpd_INV_OUT_SHIFT))         |\
         ((uint32)(PWMSpd_PWM_OUT_N_INVERT        << PWMSpd_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(PWMSpd_PWM_MODE                << PWMSpd_MODE_SHIFT)))

#define PWMSpd_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(PWMSpd_PWM_RUN_MODE         << PWMSpd_ONESHOT_SHIFT))
            
#define PWMSpd_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(PWMSpd_PWM_ALIGN            << PWMSpd_UPDOWN_SHIFT))

#define PWMSpd_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(PWMSpd_PWM_KILL_EVENT      << PWMSpd_PWM_SYNC_KILL_SHIFT))

#define PWMSpd_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(PWMSpd_PWM_DEAD_TIME_CYCLE  << PWMSpd_PRESCALER_SHIFT))

#define PWMSpd_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(PWMSpd_PWM_PRESCALER        << PWMSpd_PRESCALER_SHIFT))

#define PWMSpd_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(PWMSpd_TC_PRESCALER            << PWMSpd_PRESCALER_SHIFT))       |\
         ((uint32)(PWMSpd_TC_COUNTER_MODE         << PWMSpd_UPDOWN_SHIFT))          |\
         ((uint32)(PWMSpd_TC_RUN_MODE             << PWMSpd_ONESHOT_SHIFT))         |\
         ((uint32)(PWMSpd_TC_COMP_CAP_MODE        << PWMSpd_MODE_SHIFT)))
        
#define PWMSpd_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(PWMSpd_QUAD_PHIA_SIGNAL_MODE   << PWMSpd_COUNT_SHIFT))           |\
         ((uint32)(PWMSpd_QUAD_INDEX_SIGNAL_MODE  << PWMSpd_RELOAD_SHIFT))          |\
         ((uint32)(PWMSpd_QUAD_STOP_SIGNAL_MODE   << PWMSpd_STOP_SHIFT))            |\
         ((uint32)(PWMSpd_QUAD_PHIB_SIGNAL_MODE   << PWMSpd_START_SHIFT)))

#define PWMSpd_PWM_SIGNALS_MODES                                                              \
        (((uint32)(PWMSpd_PWM_SWITCH_SIGNAL_MODE  << PWMSpd_CAPTURE_SHIFT))         |\
         ((uint32)(PWMSpd_PWM_COUNT_SIGNAL_MODE   << PWMSpd_COUNT_SHIFT))           |\
         ((uint32)(PWMSpd_PWM_RELOAD_SIGNAL_MODE  << PWMSpd_RELOAD_SHIFT))          |\
         ((uint32)(PWMSpd_PWM_STOP_SIGNAL_MODE    << PWMSpd_STOP_SHIFT))            |\
         ((uint32)(PWMSpd_PWM_START_SIGNAL_MODE   << PWMSpd_START_SHIFT)))

#define PWMSpd_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(PWMSpd_TC_CAPTURE_SIGNAL_MODE  << PWMSpd_CAPTURE_SHIFT))         |\
         ((uint32)(PWMSpd_TC_COUNT_SIGNAL_MODE    << PWMSpd_COUNT_SHIFT))           |\
         ((uint32)(PWMSpd_TC_RELOAD_SIGNAL_MODE   << PWMSpd_RELOAD_SHIFT))          |\
         ((uint32)(PWMSpd_TC_STOP_SIGNAL_MODE     << PWMSpd_STOP_SHIFT))            |\
         ((uint32)(PWMSpd_TC_START_SIGNAL_MODE    << PWMSpd_START_SHIFT)))
        
#define PWMSpd_TIMER_UPDOWN_CNT_USED                                                          \
                ((PWMSpd__COUNT_UPDOWN0 == PWMSpd_TC_COUNTER_MODE)                  ||\
                 (PWMSpd__COUNT_UPDOWN1 == PWMSpd_TC_COUNTER_MODE))

#define PWMSpd_PWM_UPDOWN_CNT_USED                                                            \
                ((PWMSpd__CENTER == PWMSpd_PWM_ALIGN)                               ||\
                 (PWMSpd__ASYMMETRIC == PWMSpd_PWM_ALIGN))               
        
#define PWMSpd_PWM_PR_INIT_VALUE              (1u)
#define PWMSpd_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_PWMSpd_H */

/* [] END OF FILE */
