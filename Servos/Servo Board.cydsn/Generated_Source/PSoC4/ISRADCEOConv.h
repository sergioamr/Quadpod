/*******************************************************************************
* File Name: ISRADCEOConv.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_ISRADCEOConv_H)
#define CY_ISR_ISRADCEOConv_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void ISRADCEOConv_Start(void);
void ISRADCEOConv_StartEx(cyisraddress address);
void ISRADCEOConv_Stop(void);

CY_ISR_PROTO(ISRADCEOConv_Interrupt);

void ISRADCEOConv_SetVector(cyisraddress address);
cyisraddress ISRADCEOConv_GetVector(void);

void ISRADCEOConv_SetPriority(uint8 priority);
uint8 ISRADCEOConv_GetPriority(void);

void ISRADCEOConv_Enable(void);
uint8 ISRADCEOConv_GetState(void);
void ISRADCEOConv_Disable(void);

void ISRADCEOConv_SetPending(void);
void ISRADCEOConv_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the ISRADCEOConv ISR. */
#define ISRADCEOConv_INTC_VECTOR            ((reg32 *) ISRADCEOConv__INTC_VECT)

/* Address of the ISRADCEOConv ISR priority. */
#define ISRADCEOConv_INTC_PRIOR             ((reg32 *) ISRADCEOConv__INTC_PRIOR_REG)

/* Priority of the ISRADCEOConv interrupt. */
#define ISRADCEOConv_INTC_PRIOR_NUMBER      ISRADCEOConv__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable ISRADCEOConv interrupt. */
#define ISRADCEOConv_INTC_SET_EN            ((reg32 *) ISRADCEOConv__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the ISRADCEOConv interrupt. */
#define ISRADCEOConv_INTC_CLR_EN            ((reg32 *) ISRADCEOConv__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the ISRADCEOConv interrupt state to pending. */
#define ISRADCEOConv_INTC_SET_PD            ((reg32 *) ISRADCEOConv__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the ISRADCEOConv interrupt. */
#define ISRADCEOConv_INTC_CLR_PD            ((reg32 *) ISRADCEOConv__INTC_CLR_PD_REG)



#endif /* CY_ISR_ISRADCEOConv_H */


/* [] END OF FILE */
