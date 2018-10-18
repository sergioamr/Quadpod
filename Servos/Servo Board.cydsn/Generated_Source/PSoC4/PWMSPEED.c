/*******************************************************************************
* File Name: PWMSpeed.c
* Version 3.30
*
* Description:
*  The PWM User Module consist of an 8 or 16-bit counter with two 8 or 16-bit
*  comparitors. Each instance of this user module is capable of generating
*  two PWM outputs with the same period. The pulse width is selectable between
*  1 and 255/65535. The period is selectable between 2 and 255/65536 clocks.
*  The compare value output may be configured to be active when the present
*  counter is less than or less than/equal to the compare value.
*  A terminal count output is also provided. It generates a pulse one clock
*  width wide when the counter is equal to zero.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "PWMSpeed.h"

/* Error message for removed <resource> through optimization */
#ifdef PWMSpeed_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* PWMSpeed_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 PWMSpeed_initVar = 0u;


/*******************************************************************************
* Function Name: PWMSpeed_Start
********************************************************************************
*
* Summary:
*  The start function initializes the pwm with the default values, the
*  enables the counter to begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWMSpeed_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void PWMSpeed_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(PWMSpeed_initVar == 0u)
    {
        PWMSpeed_Init();
        PWMSpeed_initVar = 1u;
    }
    PWMSpeed_Enable();

}


/*******************************************************************************
* Function Name: PWMSpeed_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  PWMSpeed_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWMSpeed_Init(void) 
{
    #if (PWMSpeed_UsingFixedFunction || PWMSpeed_UseControl)
        uint8 ctrl;
    #endif /* (PWMSpeed_UsingFixedFunction || PWMSpeed_UseControl) */

    #if(!PWMSpeed_UsingFixedFunction)
        #if(PWMSpeed_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 PWMSpeed_interruptState;
        #endif /* (PWMSpeed_UseStatus) */
    #endif /* (!PWMSpeed_UsingFixedFunction) */

    #if (PWMSpeed_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        PWMSpeed_CONTROL |= PWMSpeed_CFG0_MODE;
        #if (PWMSpeed_DeadBand2_4)
            PWMSpeed_CONTROL |= PWMSpeed_CFG0_DB;
        #endif /* (PWMSpeed_DeadBand2_4) */

        ctrl = PWMSpeed_CONTROL3 & ((uint8 )(~PWMSpeed_CTRL_CMPMODE1_MASK));
        PWMSpeed_CONTROL3 = ctrl | PWMSpeed_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        PWMSpeed_RT1 &= ((uint8)(~PWMSpeed_RT1_MASK));
        PWMSpeed_RT1 |= PWMSpeed_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        PWMSpeed_RT1 &= ((uint8)(~PWMSpeed_SYNCDSI_MASK));
        PWMSpeed_RT1 |= PWMSpeed_SYNCDSI_EN;

    #elif (PWMSpeed_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = PWMSpeed_CONTROL & ((uint8)(~PWMSpeed_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~PWMSpeed_CTRL_CMPMODE1_MASK));
        PWMSpeed_CONTROL = ctrl | PWMSpeed_DEFAULT_COMPARE2_MODE |
                                   PWMSpeed_DEFAULT_COMPARE1_MODE;
    #endif /* (PWMSpeed_UsingFixedFunction) */

    #if (!PWMSpeed_UsingFixedFunction)
        #if (PWMSpeed_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            PWMSpeed_AUX_CONTROLDP0 |= (PWMSpeed_AUX_CTRL_FIFO0_CLR);
        #else /* (PWMSpeed_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            PWMSpeed_AUX_CONTROLDP0 |= (PWMSpeed_AUX_CTRL_FIFO0_CLR);
            PWMSpeed_AUX_CONTROLDP1 |= (PWMSpeed_AUX_CTRL_FIFO0_CLR);
        #endif /* (PWMSpeed_Resolution == 8) */

        PWMSpeed_WriteCounter(PWMSpeed_INIT_PERIOD_VALUE);
    #endif /* (!PWMSpeed_UsingFixedFunction) */

    PWMSpeed_WritePeriod(PWMSpeed_INIT_PERIOD_VALUE);

        #if (PWMSpeed_UseOneCompareMode)
            PWMSpeed_WriteCompare(PWMSpeed_INIT_COMPARE_VALUE1);
        #else
            PWMSpeed_WriteCompare1(PWMSpeed_INIT_COMPARE_VALUE1);
            PWMSpeed_WriteCompare2(PWMSpeed_INIT_COMPARE_VALUE2);
        #endif /* (PWMSpeed_UseOneCompareMode) */

        #if (PWMSpeed_KillModeMinTime)
            PWMSpeed_WriteKillTime(PWMSpeed_MinimumKillTime);
        #endif /* (PWMSpeed_KillModeMinTime) */

        #if (PWMSpeed_DeadBandUsed)
            PWMSpeed_WriteDeadTime(PWMSpeed_INIT_DEAD_TIME);
        #endif /* (PWMSpeed_DeadBandUsed) */

    #if (PWMSpeed_UseStatus || PWMSpeed_UsingFixedFunction)
        PWMSpeed_SetInterruptMode(PWMSpeed_INIT_INTERRUPTS_MODE);
    #endif /* (PWMSpeed_UseStatus || PWMSpeed_UsingFixedFunction) */

    #if (PWMSpeed_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        PWMSpeed_GLOBAL_ENABLE |= PWMSpeed_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        PWMSpeed_CONTROL2 |= PWMSpeed_CTRL2_IRQ_SEL;
    #else
        #if(PWMSpeed_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            PWMSpeed_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            PWMSpeed_STATUS_AUX_CTRL |= PWMSpeed_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(PWMSpeed_interruptState);

            /* Clear the FIFO to enable the PWMSpeed_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            PWMSpeed_ClearFIFO();
        #endif /* (PWMSpeed_UseStatus) */
    #endif /* (PWMSpeed_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWMSpeed_Enable
********************************************************************************
*
* Summary:
*  Enables the PWM block operation
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This works only if software enable mode is chosen
*
*******************************************************************************/
void PWMSpeed_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (PWMSpeed_UsingFixedFunction)
        PWMSpeed_GLOBAL_ENABLE |= PWMSpeed_BLOCK_EN_MASK;
        PWMSpeed_GLOBAL_STBY_ENABLE |= PWMSpeed_BLOCK_STBY_EN_MASK;
    #endif /* (PWMSpeed_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (PWMSpeed_UseControl || PWMSpeed_UsingFixedFunction)
        PWMSpeed_CONTROL |= PWMSpeed_CTRL_ENABLE;
    #endif /* (PWMSpeed_UseControl || PWMSpeed_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWMSpeed_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the PWM, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  If the Enable mode is set to Hardware only then this function
*  has no effect on the operation of the PWM
*
*******************************************************************************/
void PWMSpeed_Stop(void) 
{
    #if (PWMSpeed_UseControl || PWMSpeed_UsingFixedFunction)
        PWMSpeed_CONTROL &= ((uint8)(~PWMSpeed_CTRL_ENABLE));
    #endif /* (PWMSpeed_UseControl || PWMSpeed_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (PWMSpeed_UsingFixedFunction)
        PWMSpeed_GLOBAL_ENABLE &= ((uint8)(~PWMSpeed_BLOCK_EN_MASK));
        PWMSpeed_GLOBAL_STBY_ENABLE &= ((uint8)(~PWMSpeed_BLOCK_STBY_EN_MASK));
    #endif /* (PWMSpeed_UsingFixedFunction) */
}

#if (PWMSpeed_UseOneCompareMode)
    #if (PWMSpeed_CompareMode1SW)


        /*******************************************************************************
        * Function Name: PWMSpeed_SetCompareMode
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm output when in Dither mode,
        *  Center Align Mode or One Output Mode.
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void PWMSpeed_SetCompareMode(uint8 comparemode) 
        {
            #if(PWMSpeed_UsingFixedFunction)

                #if(0 != PWMSpeed_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << PWMSpeed_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != PWMSpeed_CTRL_CMPMODE1_SHIFT) */

                PWMSpeed_CONTROL3 &= ((uint8)(~PWMSpeed_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                PWMSpeed_CONTROL3 |= comparemodemasked;

            #elif (PWMSpeed_UseControl)

                #if(0 != PWMSpeed_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << PWMSpeed_CTRL_CMPMODE1_SHIFT)) &
                                                PWMSpeed_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & PWMSpeed_CTRL_CMPMODE1_MASK;
                #endif /* (0 != PWMSpeed_CTRL_CMPMODE1_SHIFT) */

                #if(0 != PWMSpeed_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << PWMSpeed_CTRL_CMPMODE2_SHIFT)) &
                                               PWMSpeed_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & PWMSpeed_CTRL_CMPMODE2_MASK;
                #endif /* (0 != PWMSpeed_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                PWMSpeed_CONTROL &= ((uint8)(~(PWMSpeed_CTRL_CMPMODE1_MASK |
                                            PWMSpeed_CTRL_CMPMODE2_MASK)));
                PWMSpeed_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (PWMSpeed_UsingFixedFunction) */
        }
    #endif /* PWMSpeed_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (PWMSpeed_CompareMode1SW)


        /*******************************************************************************
        * Function Name: PWMSpeed_SetCompareMode1
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm or pwm1 output
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void PWMSpeed_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != PWMSpeed_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << PWMSpeed_CTRL_CMPMODE1_SHIFT)) &
                                           PWMSpeed_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & PWMSpeed_CTRL_CMPMODE1_MASK;
            #endif /* (0 != PWMSpeed_CTRL_CMPMODE1_SHIFT) */

            #if (PWMSpeed_UseControl)
                PWMSpeed_CONTROL &= ((uint8)(~PWMSpeed_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                PWMSpeed_CONTROL |= comparemodemasked;
            #endif /* (PWMSpeed_UseControl) */
        }
    #endif /* PWMSpeed_CompareMode1SW */

#if (PWMSpeed_CompareMode2SW)


    /*******************************************************************************
    * Function Name: PWMSpeed_SetCompareMode2
    ********************************************************************************
    *
    * Summary:
    *  This function writes the Compare Mode for the pwm or pwm2 output
    *
    * Parameters:
    *  comparemode:  The new compare mode for the PWM output. Use the compare types
    *                defined in the H file as input arguments.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void PWMSpeed_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != PWMSpeed_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << PWMSpeed_CTRL_CMPMODE2_SHIFT)) &
                                                 PWMSpeed_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & PWMSpeed_CTRL_CMPMODE2_MASK;
        #endif /* (0 != PWMSpeed_CTRL_CMPMODE2_SHIFT) */

        #if (PWMSpeed_UseControl)
            PWMSpeed_CONTROL &= ((uint8)(~PWMSpeed_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            PWMSpeed_CONTROL |= comparemodemasked;
        #endif /* (PWMSpeed_UseControl) */
    }
    #endif /*PWMSpeed_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!PWMSpeed_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: PWMSpeed_WriteCounter
    ********************************************************************************
    *
    * Summary:
    *  Writes a new counter value directly to the counter register. This will be
    *  implemented for that currently running period and only that period. This API
    *  is valid only for UDB implementation and not available for fixed function
    *  PWM implementation.
    *
    * Parameters:
    *  counter:  The period new period counter value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  The PWM Period will be reloaded when a counter value will be a zero
    *
    *******************************************************************************/
    void PWMSpeed_WriteCounter(uint16 counter) \
                                       
    {
        CY_SET_REG16(PWMSpeed_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: PWMSpeed_ReadCounter
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current value of the counter.  It doesn't matter
    *  if the counter is enabled or running.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  The current value of the counter.
    *
    *******************************************************************************/
    uint16 PWMSpeed_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(PWMSpeed_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG16(PWMSpeed_CAPTURE_LSB_PTR));
    }

    #if (PWMSpeed_UseStatus)


        /*******************************************************************************
        * Function Name: PWMSpeed_ClearFIFO
        ********************************************************************************
        *
        * Summary:
        *  This function clears all capture data from the capture FIFO
        *
        * Parameters:
        *  None
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void PWMSpeed_ClearFIFO(void) 
        {
            while(0u != (PWMSpeed_ReadStatusRegister() & PWMSpeed_STATUS_FIFONEMPTY))
            {
                (void)PWMSpeed_ReadCapture();
            }
        }

    #endif /* PWMSpeed_UseStatus */

#endif /* !PWMSpeed_UsingFixedFunction */


/*******************************************************************************
* Function Name: PWMSpeed_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period:  Period value. May be between 1 and (2^Resolution)-1.  A value of 0
*           will result in the counter remaining at zero.
*
* Return:
*  None
*
*******************************************************************************/
void PWMSpeed_WritePeriod(uint16 period) 
{
    #if(PWMSpeed_UsingFixedFunction)
        CY_SET_REG16(PWMSpeed_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG16(PWMSpeed_PERIOD_LSB_PTR, period);
    #endif /* (PWMSpeed_UsingFixedFunction) */
}

#if (PWMSpeed_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: PWMSpeed_WriteCompare
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value when the PWM is in Dither
    *  mode. The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  compared to the compare value based on the compare mode defined in
    *  Dither Mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  This function is only available if the PWM mode parameter is set to
    *  Dither Mode, Center Aligned Mode or One Output Mode
    *
    *******************************************************************************/
    void PWMSpeed_WriteCompare(uint16 compare) \
                                       
    {
        #if(PWMSpeed_UsingFixedFunction)
            CY_SET_REG16(PWMSpeed_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(PWMSpeed_COMPARE1_LSB_PTR, compare);
        #endif /* (PWMSpeed_UsingFixedFunction) */

        #if (PWMSpeed_PWMMode == PWMSpeed__B_PWM__DITHER)
            #if(PWMSpeed_UsingFixedFunction)
                CY_SET_REG16(PWMSpeed_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG16(PWMSpeed_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (PWMSpeed_UsingFixedFunction) */
        #endif /* (PWMSpeed_PWMMode == PWMSpeed__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: PWMSpeed_WriteCompare1
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value.  The compare output will
    *  reflect the new value on the next UDB clock.  The compare output will be
    *  driven high when the present counter value is less than or less than or
    *  equal to the compare register, depending on the mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void PWMSpeed_WriteCompare1(uint16 compare) \
                                        
    {
        #if(PWMSpeed_UsingFixedFunction)
            CY_SET_REG16(PWMSpeed_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(PWMSpeed_COMPARE1_LSB_PTR, compare);
        #endif /* (PWMSpeed_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: PWMSpeed_WriteCompare2
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare value, for compare1 output.
    *  The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  less than or less than or equal to the compare register, depending on the
    *  mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void PWMSpeed_WriteCompare2(uint16 compare) \
                                        
    {
        #if(PWMSpeed_UsingFixedFunction)
            CY_SET_REG16(PWMSpeed_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG16(PWMSpeed_COMPARE2_LSB_PTR, compare);
        #endif /* (PWMSpeed_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (PWMSpeed_DeadBandUsed)


    /*******************************************************************************
    * Function Name: PWMSpeed_WriteDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function writes the dead-band counts to the corresponding register
    *
    * Parameters:
    *  deadtime:  Number of counts for dead time
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void PWMSpeed_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!PWMSpeed_DeadBand2_4)
            CY_SET_REG8(PWMSpeed_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            PWMSpeed_DEADBAND_COUNT &= ((uint8)(~PWMSpeed_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(PWMSpeed_DEADBAND_COUNT_SHIFT)
                PWMSpeed_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << PWMSpeed_DEADBAND_COUNT_SHIFT)) &
                                                    PWMSpeed_DEADBAND_COUNT_MASK;
            #else
                PWMSpeed_DEADBAND_COUNT |= deadtime & PWMSpeed_DEADBAND_COUNT_MASK;
            #endif /* (PWMSpeed_DEADBAND_COUNT_SHIFT) */

        #endif /* (!PWMSpeed_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: PWMSpeed_ReadDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function reads the dead-band counts from the corresponding register
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Dead Band Counts
    *
    *******************************************************************************/
    uint8 PWMSpeed_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!PWMSpeed_DeadBand2_4)
            return (CY_GET_REG8(PWMSpeed_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(PWMSpeed_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(PWMSpeed_DEADBAND_COUNT & PWMSpeed_DEADBAND_COUNT_MASK)) >>
                                                                           PWMSpeed_DEADBAND_COUNT_SHIFT));
            #else
                return (PWMSpeed_DEADBAND_COUNT & PWMSpeed_DEADBAND_COUNT_MASK);
            #endif /* (PWMSpeed_DEADBAND_COUNT_SHIFT) */
        #endif /* (!PWMSpeed_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (PWMSpeed_UseStatus || PWMSpeed_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: PWMSpeed_SetInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  This function configures the interrupts mask control of theinterrupt
    *  source status register.
    *
    * Parameters:
    *  uint8 interruptMode: Bit field containing the interrupt sources enabled
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void PWMSpeed_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(PWMSpeed_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: PWMSpeed_ReadStatusRegister
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current state of the status register.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current status register value. The status register bits are:
    *  [7:6] : Unused(0)
    *  [5]   : Kill event output
    *  [4]   : FIFO not empty
    *  [3]   : FIFO full
    *  [2]   : Terminal count
    *  [1]   : Compare output 2
    *  [0]   : Compare output 1
    *
    *******************************************************************************/
    uint8 PWMSpeed_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(PWMSpeed_STATUS_PTR));
    }

#endif /* (PWMSpeed_UseStatus || PWMSpeed_UsingFixedFunction) */


#if (PWMSpeed_UseControl)


    /*******************************************************************************
    * Function Name: PWMSpeed_ReadControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the control register. This API is available
    *  only if the control register is not removed.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current control register value
    *
    *******************************************************************************/
    uint8 PWMSpeed_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(PWMSpeed_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: PWMSpeed_WriteControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Sets the bit field of the control register. This API is available only if
    *  the control register is not removed.
    *
    * Parameters:
    *  uint8 control: Control register bit field, The status register bits are:
    *  [7]   : PWM Enable
    *  [6]   : Reset
    *  [5:3] : Compare Mode2
    *  [2:0] : Compare Mode2
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void PWMSpeed_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(PWMSpeed_CONTROL_PTR, control);
    }

#endif /* (PWMSpeed_UseControl) */


#if (!PWMSpeed_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: PWMSpeed_ReadCapture
    ********************************************************************************
    *
    * Summary:
    *  Reads the capture value from the capture FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: The current capture value
    *
    *******************************************************************************/
    uint16 PWMSpeed_ReadCapture(void) 
    {
        return (CY_GET_REG16(PWMSpeed_CAPTURE_LSB_PTR));
    }

#endif /* (!PWMSpeed_UsingFixedFunction) */


#if (PWMSpeed_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: PWMSpeed_ReadCompare
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare output when the PWM Mode parameter is
    *  set to Dither mode, Center Aligned mode, or One Output mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value
    *
    *******************************************************************************/
    uint16 PWMSpeed_ReadCompare(void) 
    {
        #if(PWMSpeed_UsingFixedFunction)
            return ((uint16)CY_GET_REG16(PWMSpeed_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG16(PWMSpeed_COMPARE1_LSB_PTR));
        #endif /* (PWMSpeed_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: PWMSpeed_ReadCompare1
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare1 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint16 PWMSpeed_ReadCompare1(void) 
    {
        return (CY_GET_REG16(PWMSpeed_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: PWMSpeed_ReadCompare2
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare2 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint16 PWMSpeed_ReadCompare2(void) 
    {
        return (CY_GET_REG16(PWMSpeed_COMPARE2_LSB_PTR));
    }

#endif /* (PWMSpeed_UseOneCompareMode) */


/*******************************************************************************
* Function Name: PWMSpeed_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the period value used by the PWM hardware.
*
* Parameters:
*  None
*
* Return:
*  uint8/16: Period value
*
*******************************************************************************/
uint16 PWMSpeed_ReadPeriod(void) 
{
    #if(PWMSpeed_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(PWMSpeed_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG16(PWMSpeed_PERIOD_LSB_PTR));
    #endif /* (PWMSpeed_UsingFixedFunction) */
}

#if ( PWMSpeed_KillModeMinTime)


    /*******************************************************************************
    * Function Name: PWMSpeed_WriteKillTime
    ********************************************************************************
    *
    * Summary:
    *  Writes the kill time value used by the hardware when the Kill Mode
    *  is set to Minimum Time.
    *
    * Parameters:
    *  uint8: Minimum Time kill counts
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void PWMSpeed_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(PWMSpeed_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: PWMSpeed_ReadKillTime
    ********************************************************************************
    *
    * Summary:
    *  Reads the kill time value used by the hardware when the Kill Mode is set
    *  to Minimum Time.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8: The current Minimum Time kill counts
    *
    *******************************************************************************/
    uint8 PWMSpeed_ReadKillTime(void) 
    {
        return (CY_GET_REG8(PWMSpeed_KILLMODEMINTIME_PTR));
    }

#endif /* ( PWMSpeed_KillModeMinTime) */

/* [] END OF FILE */
