/*******************************************************************************
* File Name: ClkDrvr.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_ClkDrvr_H)
#define CY_CLOCK_ClkDrvr_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void ClkDrvr_Start(void) ;
void ClkDrvr_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void ClkDrvr_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void ClkDrvr_StandbyPower(uint8 state) ;
void ClkDrvr_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 ClkDrvr_GetDividerRegister(void) ;
void ClkDrvr_SetModeRegister(uint8 modeBitMask) ;
void ClkDrvr_ClearModeRegister(uint8 modeBitMask) ;
uint8 ClkDrvr_GetModeRegister(void) ;
void ClkDrvr_SetSourceRegister(uint8 clkSource) ;
uint8 ClkDrvr_GetSourceRegister(void) ;
#if defined(ClkDrvr__CFG3)
void ClkDrvr_SetPhaseRegister(uint8 clkPhase) ;
uint8 ClkDrvr_GetPhaseRegister(void) ;
#endif /* defined(ClkDrvr__CFG3) */

#define ClkDrvr_Enable()                       ClkDrvr_Start()
#define ClkDrvr_Disable()                      ClkDrvr_Stop()
#define ClkDrvr_SetDivider(clkDivider)         ClkDrvr_SetDividerRegister(clkDivider, 1u)
#define ClkDrvr_SetDividerValue(clkDivider)    ClkDrvr_SetDividerRegister((clkDivider) - 1u, 1u)
#define ClkDrvr_SetMode(clkMode)               ClkDrvr_SetModeRegister(clkMode)
#define ClkDrvr_SetSource(clkSource)           ClkDrvr_SetSourceRegister(clkSource)
#if defined(ClkDrvr__CFG3)
#define ClkDrvr_SetPhase(clkPhase)             ClkDrvr_SetPhaseRegister(clkPhase)
#define ClkDrvr_SetPhaseValue(clkPhase)        ClkDrvr_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(ClkDrvr__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define ClkDrvr_CLKEN              (* (reg8 *) ClkDrvr__PM_ACT_CFG)
#define ClkDrvr_CLKEN_PTR          ((reg8 *) ClkDrvr__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define ClkDrvr_CLKSTBY            (* (reg8 *) ClkDrvr__PM_STBY_CFG)
#define ClkDrvr_CLKSTBY_PTR        ((reg8 *) ClkDrvr__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define ClkDrvr_DIV_LSB            (* (reg8 *) ClkDrvr__CFG0)
#define ClkDrvr_DIV_LSB_PTR        ((reg8 *) ClkDrvr__CFG0)
#define ClkDrvr_DIV_PTR            ((reg16 *) ClkDrvr__CFG0)

/* Clock MSB divider configuration register. */
#define ClkDrvr_DIV_MSB            (* (reg8 *) ClkDrvr__CFG1)
#define ClkDrvr_DIV_MSB_PTR        ((reg8 *) ClkDrvr__CFG1)

/* Mode and source configuration register */
#define ClkDrvr_MOD_SRC            (* (reg8 *) ClkDrvr__CFG2)
#define ClkDrvr_MOD_SRC_PTR        ((reg8 *) ClkDrvr__CFG2)

#if defined(ClkDrvr__CFG3)
/* Analog clock phase configuration register */
#define ClkDrvr_PHASE              (* (reg8 *) ClkDrvr__CFG3)
#define ClkDrvr_PHASE_PTR          ((reg8 *) ClkDrvr__CFG3)
#endif /* defined(ClkDrvr__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define ClkDrvr_CLKEN_MASK         ClkDrvr__PM_ACT_MSK
#define ClkDrvr_CLKSTBY_MASK       ClkDrvr__PM_STBY_MSK

/* CFG2 field masks */
#define ClkDrvr_SRC_SEL_MSK        ClkDrvr__CFG2_SRC_SEL_MASK
#define ClkDrvr_MODE_MASK          (~(ClkDrvr_SRC_SEL_MSK))

#if defined(ClkDrvr__CFG3)
/* CFG3 phase mask */
#define ClkDrvr_PHASE_MASK         ClkDrvr__CFG3_PHASE_DLY_MASK
#endif /* defined(ClkDrvr__CFG3) */

#endif /* CY_CLOCK_ClkDrvr_H */


/* [] END OF FILE */
