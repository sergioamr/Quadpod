/*******************************************************************************
* File Name: PWMA_PM.c
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

#include "PWMA.h"

static PWMA_backupStruct PWMA_backup;


/*******************************************************************************
* Function Name: PWMA_SaveConfig
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
*  PWMA_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWMA_SaveConfig(void) 
{

    #if(!PWMA_UsingFixedFunction)
        #if(!PWMA_PWMModeIsCenterAligned)
            PWMA_backup.PWMPeriod = PWMA_ReadPeriod();
        #endif /* (!PWMA_PWMModeIsCenterAligned) */
        PWMA_backup.PWMUdb = PWMA_ReadCounter();
        #if (PWMA_UseStatus)
            PWMA_backup.InterruptMaskValue = PWMA_STATUS_MASK;
        #endif /* (PWMA_UseStatus) */

        #if(PWMA_DeadBandMode == PWMA__B_PWM__DBM_256_CLOCKS || \
            PWMA_DeadBandMode == PWMA__B_PWM__DBM_2_4_CLOCKS)
            PWMA_backup.PWMdeadBandValue = PWMA_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWMA_KillModeMinTime)
             PWMA_backup.PWMKillCounterPeriod = PWMA_ReadKillTime();
        #endif /* (PWMA_KillModeMinTime) */

        #if(PWMA_UseControl)
            PWMA_backup.PWMControlRegister = PWMA_ReadControlRegister();
        #endif /* (PWMA_UseControl) */
    #endif  /* (!PWMA_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWMA_RestoreConfig
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
*  PWMA_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWMA_RestoreConfig(void) 
{
        #if(!PWMA_UsingFixedFunction)
            #if(!PWMA_PWMModeIsCenterAligned)
                PWMA_WritePeriod(PWMA_backup.PWMPeriod);
            #endif /* (!PWMA_PWMModeIsCenterAligned) */

            PWMA_WriteCounter(PWMA_backup.PWMUdb);

            #if (PWMA_UseStatus)
                PWMA_STATUS_MASK = PWMA_backup.InterruptMaskValue;
            #endif /* (PWMA_UseStatus) */

            #if(PWMA_DeadBandMode == PWMA__B_PWM__DBM_256_CLOCKS || \
                PWMA_DeadBandMode == PWMA__B_PWM__DBM_2_4_CLOCKS)
                PWMA_WriteDeadTime(PWMA_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWMA_KillModeMinTime)
                PWMA_WriteKillTime(PWMA_backup.PWMKillCounterPeriod);
            #endif /* (PWMA_KillModeMinTime) */

            #if(PWMA_UseControl)
                PWMA_WriteControlRegister(PWMA_backup.PWMControlRegister);
            #endif /* (PWMA_UseControl) */
        #endif  /* (!PWMA_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWMA_Sleep
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
*  PWMA_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWMA_Sleep(void) 
{
    #if(PWMA_UseControl)
        if(PWMA_CTRL_ENABLE == (PWMA_CONTROL & PWMA_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWMA_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWMA_backup.PWMEnableState = 0u;
        }
    #endif /* (PWMA_UseControl) */

    /* Stop component */
    PWMA_Stop();

    /* Save registers configuration */
    PWMA_SaveConfig();
}


/*******************************************************************************
* Function Name: PWMA_Wakeup
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
*  PWMA_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWMA_Wakeup(void) 
{
     /* Restore registers values */
    PWMA_RestoreConfig();

    if(PWMA_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWMA_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
