/*******************************************************************************
* File Name: PWMB_PM.c
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

#include "PWMB.h"

static PWMB_backupStruct PWMB_backup;


/*******************************************************************************
* Function Name: PWMB_SaveConfig
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
*  PWMB_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWMB_SaveConfig(void) 
{

    #if(!PWMB_UsingFixedFunction)
        #if(!PWMB_PWMModeIsCenterAligned)
            PWMB_backup.PWMPeriod = PWMB_ReadPeriod();
        #endif /* (!PWMB_PWMModeIsCenterAligned) */
        PWMB_backup.PWMUdb = PWMB_ReadCounter();
        #if (PWMB_UseStatus)
            PWMB_backup.InterruptMaskValue = PWMB_STATUS_MASK;
        #endif /* (PWMB_UseStatus) */

        #if(PWMB_DeadBandMode == PWMB__B_PWM__DBM_256_CLOCKS || \
            PWMB_DeadBandMode == PWMB__B_PWM__DBM_2_4_CLOCKS)
            PWMB_backup.PWMdeadBandValue = PWMB_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWMB_KillModeMinTime)
             PWMB_backup.PWMKillCounterPeriod = PWMB_ReadKillTime();
        #endif /* (PWMB_KillModeMinTime) */

        #if(PWMB_UseControl)
            PWMB_backup.PWMControlRegister = PWMB_ReadControlRegister();
        #endif /* (PWMB_UseControl) */
    #endif  /* (!PWMB_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWMB_RestoreConfig
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
*  PWMB_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWMB_RestoreConfig(void) 
{
        #if(!PWMB_UsingFixedFunction)
            #if(!PWMB_PWMModeIsCenterAligned)
                PWMB_WritePeriod(PWMB_backup.PWMPeriod);
            #endif /* (!PWMB_PWMModeIsCenterAligned) */

            PWMB_WriteCounter(PWMB_backup.PWMUdb);

            #if (PWMB_UseStatus)
                PWMB_STATUS_MASK = PWMB_backup.InterruptMaskValue;
            #endif /* (PWMB_UseStatus) */

            #if(PWMB_DeadBandMode == PWMB__B_PWM__DBM_256_CLOCKS || \
                PWMB_DeadBandMode == PWMB__B_PWM__DBM_2_4_CLOCKS)
                PWMB_WriteDeadTime(PWMB_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWMB_KillModeMinTime)
                PWMB_WriteKillTime(PWMB_backup.PWMKillCounterPeriod);
            #endif /* (PWMB_KillModeMinTime) */

            #if(PWMB_UseControl)
                PWMB_WriteControlRegister(PWMB_backup.PWMControlRegister);
            #endif /* (PWMB_UseControl) */
        #endif  /* (!PWMB_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWMB_Sleep
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
*  PWMB_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWMB_Sleep(void) 
{
    #if(PWMB_UseControl)
        if(PWMB_CTRL_ENABLE == (PWMB_CONTROL & PWMB_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWMB_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWMB_backup.PWMEnableState = 0u;
        }
    #endif /* (PWMB_UseControl) */

    /* Stop component */
    PWMB_Stop();

    /* Save registers configuration */
    PWMB_SaveConfig();
}


/*******************************************************************************
* Function Name: PWMB_Wakeup
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
*  PWMB_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWMB_Wakeup(void) 
{
     /* Restore registers values */
    PWMB_RestoreConfig();

    if(PWMB_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWMB_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
