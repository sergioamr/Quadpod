/*******************************************************************************
* File Name: PWMSpeed_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PWMSpeed.h"

static PWMSpeed_backupStruct PWMSpeed_backup;


/*******************************************************************************
* Function Name: PWMSpeed_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWMSpeed_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWMSpeed_SaveConfig(void) 
{

    #if(!PWMSpeed_UsingFixedFunction)
        #if(!PWMSpeed_PWMModeIsCenterAligned)
            PWMSpeed_backup.PWMPeriod = PWMSpeed_ReadPeriod();
        #endif /* (!PWMSpeed_PWMModeIsCenterAligned) */
        PWMSpeed_backup.PWMUdb = PWMSpeed_ReadCounter();
        #if (PWMSpeed_UseStatus)
            PWMSpeed_backup.InterruptMaskValue = PWMSpeed_STATUS_MASK;
        #endif /* (PWMSpeed_UseStatus) */

        #if(PWMSpeed_DeadBandMode == PWMSpeed__B_PWM__DBM_256_CLOCKS || \
            PWMSpeed_DeadBandMode == PWMSpeed__B_PWM__DBM_2_4_CLOCKS)
            PWMSpeed_backup.PWMdeadBandValue = PWMSpeed_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWMSpeed_KillModeMinTime)
             PWMSpeed_backup.PWMKillCounterPeriod = PWMSpeed_ReadKillTime();
        #endif /* (PWMSpeed_KillModeMinTime) */

        #if(PWMSpeed_UseControl)
            PWMSpeed_backup.PWMControlRegister = PWMSpeed_ReadControlRegister();
        #endif /* (PWMSpeed_UseControl) */
    #endif  /* (!PWMSpeed_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWMSpeed_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWMSpeed_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWMSpeed_RestoreConfig(void) 
{
        #if(!PWMSpeed_UsingFixedFunction)
            #if(!PWMSpeed_PWMModeIsCenterAligned)
                PWMSpeed_WritePeriod(PWMSpeed_backup.PWMPeriod);
            #endif /* (!PWMSpeed_PWMModeIsCenterAligned) */

            PWMSpeed_WriteCounter(PWMSpeed_backup.PWMUdb);

            #if (PWMSpeed_UseStatus)
                PWMSpeed_STATUS_MASK = PWMSpeed_backup.InterruptMaskValue;
            #endif /* (PWMSpeed_UseStatus) */

            #if(PWMSpeed_DeadBandMode == PWMSpeed__B_PWM__DBM_256_CLOCKS || \
                PWMSpeed_DeadBandMode == PWMSpeed__B_PWM__DBM_2_4_CLOCKS)
                PWMSpeed_WriteDeadTime(PWMSpeed_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWMSpeed_KillModeMinTime)
                PWMSpeed_WriteKillTime(PWMSpeed_backup.PWMKillCounterPeriod);
            #endif /* (PWMSpeed_KillModeMinTime) */

            #if(PWMSpeed_UseControl)
                PWMSpeed_WriteControlRegister(PWMSpeed_backup.PWMControlRegister);
            #endif /* (PWMSpeed_UseControl) */
        #endif  /* (!PWMSpeed_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWMSpeed_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWMSpeed_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWMSpeed_Sleep(void) 
{
    #if(PWMSpeed_UseControl)
        if(PWMSpeed_CTRL_ENABLE == (PWMSpeed_CONTROL & PWMSpeed_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWMSpeed_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWMSpeed_backup.PWMEnableState = 0u;
        }
    #endif /* (PWMSpeed_UseControl) */

    /* Stop component */
    PWMSpeed_Stop();

    /* Save registers configuration */
    PWMSpeed_SaveConfig();
}


/*******************************************************************************
* Function Name: PWMSpeed_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWMSpeed_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWMSpeed_Wakeup(void) 
{
     /* Restore registers values */
    PWMSpeed_RestoreConfig();

    if(PWMSpeed_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWMSpeed_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
