/*******************************************************************************
* File Name: VDD.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "VDD.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        VDD_PC =   (VDD_PC & \
                                (uint32)(~(uint32)(VDD_DRIVE_MODE_IND_MASK << (VDD_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (VDD_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: VDD_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None 
*  
*******************************************************************************/
void VDD_Write(uint8 value) 
{
    uint8 drVal = (uint8)(VDD_DR & (uint8)(~VDD_MASK));
    drVal = (drVal | ((uint8)(value << VDD_SHIFT) & VDD_MASK));
    VDD_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: VDD_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  VDD_DM_STRONG     Strong Drive 
*  VDD_DM_OD_HI      Open Drain, Drives High 
*  VDD_DM_OD_LO      Open Drain, Drives Low 
*  VDD_DM_RES_UP     Resistive Pull Up 
*  VDD_DM_RES_DWN    Resistive Pull Down 
*  VDD_DM_RES_UPDWN  Resistive Pull Up/Down 
*  VDD_DM_DIG_HIZ    High Impedance Digital 
*  VDD_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void VDD_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(VDD__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: VDD_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro VDD_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 VDD_Read(void) 
{
    return (uint8)((VDD_PS & VDD_MASK) >> VDD_SHIFT);
}


/*******************************************************************************
* Function Name: VDD_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 VDD_ReadDataReg(void) 
{
    return (uint8)((VDD_DR & VDD_MASK) >> VDD_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(VDD_INTSTAT) 

    /*******************************************************************************
    * Function Name: VDD_ClearInterrupt
    ********************************************************************************
    *
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 VDD_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(VDD_INTSTAT & VDD_MASK);
		VDD_INTSTAT = maskedStatus;
        return maskedStatus >> VDD_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
