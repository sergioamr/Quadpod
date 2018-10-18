/*******************************************************************************
* File Name: MotorIn2.c  
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
#include "MotorIn2.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        MotorIn2_PC =   (MotorIn2_PC & \
                                (uint32)(~(uint32)(MotorIn2_DRIVE_MODE_IND_MASK << (MotorIn2_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (MotorIn2_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: MotorIn2_Write
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
void MotorIn2_Write(uint8 value) 
{
    uint8 drVal = (uint8)(MotorIn2_DR & (uint8)(~MotorIn2_MASK));
    drVal = (drVal | ((uint8)(value << MotorIn2_SHIFT) & MotorIn2_MASK));
    MotorIn2_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: MotorIn2_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  MotorIn2_DM_STRONG     Strong Drive 
*  MotorIn2_DM_OD_HI      Open Drain, Drives High 
*  MotorIn2_DM_OD_LO      Open Drain, Drives Low 
*  MotorIn2_DM_RES_UP     Resistive Pull Up 
*  MotorIn2_DM_RES_DWN    Resistive Pull Down 
*  MotorIn2_DM_RES_UPDWN  Resistive Pull Up/Down 
*  MotorIn2_DM_DIG_HIZ    High Impedance Digital 
*  MotorIn2_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void MotorIn2_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(MotorIn2__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: MotorIn2_Read
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
*  Macro MotorIn2_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 MotorIn2_Read(void) 
{
    return (uint8)((MotorIn2_PS & MotorIn2_MASK) >> MotorIn2_SHIFT);
}


/*******************************************************************************
* Function Name: MotorIn2_ReadDataReg
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
uint8 MotorIn2_ReadDataReg(void) 
{
    return (uint8)((MotorIn2_DR & MotorIn2_MASK) >> MotorIn2_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(MotorIn2_INTSTAT) 

    /*******************************************************************************
    * Function Name: MotorIn2_ClearInterrupt
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
    uint8 MotorIn2_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(MotorIn2_INTSTAT & MotorIn2_MASK);
		MotorIn2_INTSTAT = maskedStatus;
        return maskedStatus >> MotorIn2_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
