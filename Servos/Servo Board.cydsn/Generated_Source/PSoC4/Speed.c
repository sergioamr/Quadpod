/*******************************************************************************
* File Name: Speed.c  
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
#include "Speed.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        Speed_PC =   (Speed_PC & \
                                (uint32)(~(uint32)(Speed_DRIVE_MODE_IND_MASK << (Speed_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (Speed_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: Speed_Write
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
void Speed_Write(uint8 value) 
{
    uint8 drVal = (uint8)(Speed_DR & (uint8)(~Speed_MASK));
    drVal = (drVal | ((uint8)(value << Speed_SHIFT) & Speed_MASK));
    Speed_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: Speed_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  Speed_DM_STRONG     Strong Drive 
*  Speed_DM_OD_HI      Open Drain, Drives High 
*  Speed_DM_OD_LO      Open Drain, Drives Low 
*  Speed_DM_RES_UP     Resistive Pull Up 
*  Speed_DM_RES_DWN    Resistive Pull Down 
*  Speed_DM_RES_UPDWN  Resistive Pull Up/Down 
*  Speed_DM_DIG_HIZ    High Impedance Digital 
*  Speed_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void Speed_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(Speed__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: Speed_Read
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
*  Macro Speed_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Speed_Read(void) 
{
    return (uint8)((Speed_PS & Speed_MASK) >> Speed_SHIFT);
}


/*******************************************************************************
* Function Name: Speed_ReadDataReg
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
uint8 Speed_ReadDataReg(void) 
{
    return (uint8)((Speed_DR & Speed_MASK) >> Speed_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Speed_INTSTAT) 

    /*******************************************************************************
    * Function Name: Speed_ClearInterrupt
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
    uint8 Speed_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(Speed_INTSTAT & Speed_MASK);
		Speed_INTSTAT = maskedStatus;
        return maskedStatus >> Speed_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
