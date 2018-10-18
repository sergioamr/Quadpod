/*******************************************************************************
* File Name: PWMSpd_PM.c
* Version 2.0
*
* Description:
*  This file contains the setup, control, and status commands to support
*  the component operations in the low power mode.
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

#include "PWMSpd.h"

static PWMSpd_BACKUP_STRUCT PWMSpd_backup;


/*******************************************************************************
* Function Name: PWMSpd_SaveConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to save here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWMSpd_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: PWMSpd_Sleep
********************************************************************************
*
* Summary:
*  Stops the component operation and saves the user configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWMSpd_Sleep(void)
{
    if(0u != (PWMSpd_BLOCK_CONTROL_REG & PWMSpd_MASK))
    {
        PWMSpd_backup.enableState = 1u;
    }
    else
    {
        PWMSpd_backup.enableState = 0u;
    }

    PWMSpd_Stop();
    PWMSpd_SaveConfig();
}


/*******************************************************************************
* Function Name: PWMSpd_RestoreConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to restore here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWMSpd_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: PWMSpd_Wakeup
********************************************************************************
*
* Summary:
*  Restores the user configuration and restores the enable state.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWMSpd_Wakeup(void)
{
    PWMSpd_RestoreConfig();

    if(0u != PWMSpd_backup.enableState)
    {
        PWMSpd_Enable();
    }
}


/* [] END OF FILE */
