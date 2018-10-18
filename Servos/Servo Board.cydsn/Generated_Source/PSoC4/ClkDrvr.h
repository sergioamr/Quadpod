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
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void ClkDrvr_StartEx(uint32 alignClkDiv);
#define ClkDrvr_Start() \
    ClkDrvr_StartEx(ClkDrvr__PA_DIV_ID)

#else

void ClkDrvr_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void ClkDrvr_Stop(void);

void ClkDrvr_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 ClkDrvr_GetDividerRegister(void);
uint8  ClkDrvr_GetFractionalDividerRegister(void);

#define ClkDrvr_Enable()                         ClkDrvr_Start()
#define ClkDrvr_Disable()                        ClkDrvr_Stop()
#define ClkDrvr_SetDividerRegister(clkDivider, reset)  \
    ClkDrvr_SetFractionalDividerRegister((clkDivider), 0u)
#define ClkDrvr_SetDivider(clkDivider)           ClkDrvr_SetDividerRegister((clkDivider), 1u)
#define ClkDrvr_SetDividerValue(clkDivider)      ClkDrvr_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define ClkDrvr_DIV_ID     ClkDrvr__DIV_ID

#define ClkDrvr_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define ClkDrvr_CTRL_REG   (*(reg32 *)ClkDrvr__CTRL_REGISTER)
#define ClkDrvr_DIV_REG    (*(reg32 *)ClkDrvr__DIV_REGISTER)

#define ClkDrvr_CMD_DIV_SHIFT          (0u)
#define ClkDrvr_CMD_PA_DIV_SHIFT       (8u)
#define ClkDrvr_CMD_DISABLE_SHIFT      (30u)
#define ClkDrvr_CMD_ENABLE_SHIFT       (31u)

#define ClkDrvr_CMD_DISABLE_MASK       ((uint32)((uint32)1u << ClkDrvr_CMD_DISABLE_SHIFT))
#define ClkDrvr_CMD_ENABLE_MASK        ((uint32)((uint32)1u << ClkDrvr_CMD_ENABLE_SHIFT))

#define ClkDrvr_DIV_FRAC_MASK  (0x000000F8u)
#define ClkDrvr_DIV_FRAC_SHIFT (3u)
#define ClkDrvr_DIV_INT_MASK   (0xFFFFFF00u)
#define ClkDrvr_DIV_INT_SHIFT  (8u)

#else 

#define ClkDrvr_DIV_REG        (*(reg32 *)ClkDrvr__REGISTER)
#define ClkDrvr_ENABLE_REG     ClkDrvr_DIV_REG
#define ClkDrvr_DIV_FRAC_MASK  ClkDrvr__FRAC_MASK
#define ClkDrvr_DIV_FRAC_SHIFT (16u)
#define ClkDrvr_DIV_INT_MASK   ClkDrvr__DIVIDER_MASK
#define ClkDrvr_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_ClkDrvr_H) */

/* [] END OF FILE */
