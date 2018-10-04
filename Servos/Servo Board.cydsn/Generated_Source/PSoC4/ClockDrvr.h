/*******************************************************************************
* File Name: ClockDrvr.h
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

#if !defined(CY_CLOCK_ClockDrvr_H)
#define CY_CLOCK_ClockDrvr_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void ClockDrvr_StartEx(uint32 alignClkDiv);
#define ClockDrvr_Start() \
    ClockDrvr_StartEx(ClockDrvr__PA_DIV_ID)

#else

void ClockDrvr_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void ClockDrvr_Stop(void);

void ClockDrvr_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 ClockDrvr_GetDividerRegister(void);
uint8  ClockDrvr_GetFractionalDividerRegister(void);

#define ClockDrvr_Enable()                         ClockDrvr_Start()
#define ClockDrvr_Disable()                        ClockDrvr_Stop()
#define ClockDrvr_SetDividerRegister(clkDivider, reset)  \
    ClockDrvr_SetFractionalDividerRegister((clkDivider), 0u)
#define ClockDrvr_SetDivider(clkDivider)           ClockDrvr_SetDividerRegister((clkDivider), 1u)
#define ClockDrvr_SetDividerValue(clkDivider)      ClockDrvr_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define ClockDrvr_DIV_ID     ClockDrvr__DIV_ID

#define ClockDrvr_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define ClockDrvr_CTRL_REG   (*(reg32 *)ClockDrvr__CTRL_REGISTER)
#define ClockDrvr_DIV_REG    (*(reg32 *)ClockDrvr__DIV_REGISTER)

#define ClockDrvr_CMD_DIV_SHIFT          (0u)
#define ClockDrvr_CMD_PA_DIV_SHIFT       (8u)
#define ClockDrvr_CMD_DISABLE_SHIFT      (30u)
#define ClockDrvr_CMD_ENABLE_SHIFT       (31u)

#define ClockDrvr_CMD_DISABLE_MASK       ((uint32)((uint32)1u << ClockDrvr_CMD_DISABLE_SHIFT))
#define ClockDrvr_CMD_ENABLE_MASK        ((uint32)((uint32)1u << ClockDrvr_CMD_ENABLE_SHIFT))

#define ClockDrvr_DIV_FRAC_MASK  (0x000000F8u)
#define ClockDrvr_DIV_FRAC_SHIFT (3u)
#define ClockDrvr_DIV_INT_MASK   (0xFFFFFF00u)
#define ClockDrvr_DIV_INT_SHIFT  (8u)

#else 

#define ClockDrvr_DIV_REG        (*(reg32 *)ClockDrvr__REGISTER)
#define ClockDrvr_ENABLE_REG     ClockDrvr_DIV_REG
#define ClockDrvr_DIV_FRAC_MASK  ClockDrvr__FRAC_MASK
#define ClockDrvr_DIV_FRAC_SHIFT (16u)
#define ClockDrvr_DIV_INT_MASK   ClockDrvr__DIVIDER_MASK
#define ClockDrvr_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_ClockDrvr_H) */

/* [] END OF FILE */
