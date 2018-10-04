/*******************************************************************************
* File Name: PWMB.c
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

#include "PWMB.h"

/* Error message for removed <resource> through optimization */
#ifdef PWMB_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* PWMB_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 PWMB_initVar = 0u;


/*******************************************************************************
* Function Name: PWMB_Start
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
*  PWMB_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void PWMB_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(PWMB_initVar == 0u)
    {
        PWMB_Init();
        PWMB_initVar = 1u;
    }
    PWMB_Enable();

}


/*******************************************************************************
* Function Name: PWMB_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  PWMB_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWMB_Init(void) 
{
    #if (PWMB_UsingFixedFunction || PWMB_UseControl)
        uint8 ctrl;
    #endif /* (PWMB_UsingFixedFunction || PWMB_UseControl) */

    #if(!PWMB_UsingFixedFunction)
        #if(PWMB_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 PWMB_interruptState;
        #endif /* (PWMB_UseStatus) */
    #endif /* (!PWMB_UsingFixedFunction) */

    #if (PWMB_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        PWMB_CONTROL |= PWMB_CFG0_MODE;
        #if (PWMB_DeadBand2_4)
            PWMB_CONTROL |= PWMB_CFG0_DB;
        #endif /* (PWMB_DeadBand2_4) */

        ctrl = PWMB_CONTROL3 & ((uint8 )(~PWMB_CTRL_CMPMODE1_MASK));
        PWMB_CONTROL3 = ctrl | PWMB_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        PWMB_RT1 &= ((uint8)(~PWMB_RT1_MASK));
        PWMB_RT1 |= PWMB_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        PWMB_RT1 &= ((uint8)(~PWMB_SYNCDSI_MASK));
        PWMB_RT1 |= PWMB_SYNCDSI_EN;

    #elif (PWMB_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = PWMB_CONTROL & ((uint8)(~PWMB_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~PWMB_CTRL_CMPMODE1_MASK));
        PWMB_CONTROL = ctrl | PWMB_DEFAULT_COMPARE2_MODE |
                                   PWMB_DEFAULT_COMPARE1_MODE;
    #endif /* (PWMB_UsingFixedFunction) */

    #if (!PWMB_UsingFixedFunction)
        #if (PWMB_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            PWMB_AUX_CONTROLDP0 |= (PWMB_AUX_CTRL_FIFO0_CLR);
        #else /* (PWMB_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            PWMB_AUX_CONTROLDP0 |= (PWMB_AUX_CTRL_FIFO0_CLR);
            PWMB_AUX_CONTROLDP1 |= (PWMB_AUX_CTRL_FIFO0_CLR);
        #endif /* (PWMB_Resolution == 8) */

        PWMB_WriteCounter(PWMB_INIT_PERIOD_VALUE);
    #endif /* (!PWMB_UsingFixedFunction) */

    PWMB_WritePeriod(PWMB_INIT_PERIOD_VALUE);

        #if (PWMB_UseOneCompareMode)
            PWMB_WriteCompare(PWMB_INIT_COMPARE_VALUE1);
        #else
            PWMB_WriteCompare1(PWMB_INIT_COMPARE_VALUE1);
            PWMB_WriteCompare2(PWMB_INIT_COMPARE_VALUE2);
        #endif /* (PWMB_UseOneCompareMode) */

        #if (PWMB_KillModeMinTime)
            PWMB_WriteKillTime(PWMB_MinimumKillTime);
        #endif /* (PWMB_KillModeMinTime) */

        #if (PWMB_DeadBandUsed)
            PWMB_WriteDeadTime(PWMB_INIT_DEAD_TIME);
        #endif /* (PWMB_DeadBandUsed) */

    #if (PWMB_UseStatus || PWMB_UsingFixedFunction)
        PWMB_SetInterruptMode(PWMB_INIT_INTERRUPTS_MODE);
    #endif /* (PWMB_UseStatus || PWMB_UsingFixedFunction) */

    #if (PWMB_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        PWMB_GLOBAL_ENABLE |= PWMB_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        PWMB_CONTROL2 |= PWMB_CTRL2_IRQ_SEL;
    #else
        #if(PWMB_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            PWMB_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            PWMB_STATUS_AUX_CTRL |= PWMB_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(PWMB_interruptState);

            /* Clear the FIFO to enable the PWMB_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            PWMB_ClearFIFO();
        #endif /* (PWMB_UseStatus) */
    #endif /* (PWMB_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWMB_Enable
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
void PWMB_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (PWMB_UsingFixedFunction)
        PWMB_GLOBAL_ENABLE |= PWMB_BLOCK_EN_MASK;
        PWMB_GLOBAL_STBY_ENABLE |= PWMB_BLOCK_STBY_EN_MASK;
    #endif /* (PWMB_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (PWMB_UseControl || PWMB_UsingFixedFunction)
        PWMB_CONTROL |= PWMB_CTRL_ENABLE;
    #endif /* (PWMB_UseControl || PWMB_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWMB_Stop
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
void PWMB_Stop(void) 
{
    #if (PWMB_UseControl || PWMB_UsingFixedFunction)
        PWMB_CONTROL &= ((uint8)(~PWMB_CTRL_ENABLE));
    #endif /* (PWMB_UseControl || PWMB_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (PWMB_UsingFixedFunction)
        PWMB_GLOBAL_ENABLE &= ((uint8)(~PWMB_BLOCK_EN_MASK));
        PWMB_GLOBAL_STBY_ENABLE &= ((uint8)(~PWMB_BLOCK_STBY_EN_MASK));
    #endif /* (PWMB_UsingFixedFunction) */
}

#if (PWMB_UseOneCompareMode)
    #if (PWMB_CompareMode1SW)


        /*******************************************************************************
        * Function Name: PWMB_SetCompareMode
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
        void PWMB_SetCompareMode(uint8 comparemode) 
        {
            #if(PWMB_UsingFixedFunction)

                #if(0 != PWMB_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << PWMB_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != PWMB_CTRL_CMPMODE1_SHIFT) */

                PWMB_CONTROL3 &= ((uint8)(~PWMB_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                PWMB_CONTROL3 |= comparemodemasked;

            #elif (PWMB_UseControl)

                #if(0 != PWMB_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << PWMB_CTRL_CMPMODE1_SHIFT)) &
                                                PWMB_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & PWMB_CTRL_CMPMODE1_MASK;
                #endif /* (0 != PWMB_CTRL_CMPMODE1_SHIFT) */

                #if(0 != PWMB_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << PWMB_CTRL_CMPMODE2_SHIFT)) &
                                               PWMB_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & PWMB_CTRL_CMPMODE2_MASK;
                #endif /* (0 != PWMB_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                PWMB_CONTROL &= ((uint8)(~(PWMB_CTRL_CMPMODE1_MASK |
                                            PWMB_CTRL_CMPMODE2_MASK)));
                PWMB_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (PWMB_UsingFixedFunction) */
        }
    #endif /* PWMB_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (PWMB_CompareMode1SW)


        /*******************************************************************************
        * Function Name: PWMB_SetCompareMode1
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
        void PWMB_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != PWMB_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << PWMB_CTRL_CMPMODE1_SHIFT)) &
                                           PWMB_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & PWMB_CTRL_CMPMODE1_MASK;
            #endif /* (0 != PWMB_CTRL_CMPMODE1_SHIFT) */

            #if (PWMB_UseControl)
                PWMB_CONTROL &= ((uint8)(~PWMB_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                PWMB_CONTROL |= comparemodemasked;
            #endif /* (PWMB_UseControl) */
        }
    #endif /* PWMB_CompareMode1SW */

#if (PWMB_CompareMode2SW)


    /*******************************************************************************
    * Function Name: PWMB_SetCompareMode2
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
    void PWMB_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != PWMB_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << PWMB_CTRL_CMPMODE2_SHIFT)) &
                                                 PWMB_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & PWMB_CTRL_CMPMODE2_MASK;
        #endif /* (0 != PWMB_CTRL_CMPMODE2_SHIFT) */

        #if (PWMB_UseControl)
            PWMB_CONTROL &= ((uint8)(~PWMB_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            PWMB_CONTROL |= comparemodemasked;
        #endif /* (PWMB_UseControl) */
    }
    #endif /*PWMB_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!PWMB_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: PWMB_WriteCounter
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
    void PWMB_WriteCounter(uint16 counter) \
                                       
    {
        CY_SET_REG16(PWMB_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: PWMB_ReadCounter
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
    uint16 PWMB_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(PWMB_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG16(PWMB_CAPTURE_LSB_PTR));
    }

    #if (PWMB_UseStatus)


        /*******************************************************************************
        * Function Name: PWMB_ClearFIFO
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
        void PWMB_ClearFIFO(void) 
        {
            while(0u != (PWMB_ReadStatusRegister() & PWMB_STATUS_FIFONEMPTY))
            {
                (void)PWMB_ReadCapture();
            }
        }

    #endif /* PWMB_UseStatus */

#endif /* !PWMB_UsingFixedFunction */


/*******************************************************************************
* Function Name: PWMB_WritePeriod
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
void PWMB_WritePeriod(uint16 period) 
{
    #if(PWMB_UsingFixedFunction)
        CY_SET_REG16(PWMB_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG16(PWMB_PERIOD_LSB_PTR, period);
    #endif /* (PWMB_UsingFixedFunction) */
}

#if (PWMB_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: PWMB_WriteCompare
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
    void PWMB_WriteCompare(uint16 compare) \
                                       
    {
        #if(PWMB_UsingFixedFunction)
            CY_SET_REG16(PWMB_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(PWMB_COMPARE1_LSB_PTR, compare);
        #endif /* (PWMB_UsingFixedFunction) */

        #if (PWMB_PWMMode == PWMB__B_PWM__DITHER)
            #if(PWMB_UsingFixedFunction)
                CY_SET_REG16(PWMB_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG16(PWMB_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (PWMB_UsingFixedFunction) */
        #endif /* (PWMB_PWMMode == PWMB__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: PWMB_WriteCompare1
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
    void PWMB_WriteCompare1(uint16 compare) \
                                        
    {
        #if(PWMB_UsingFixedFunction)
            CY_SET_REG16(PWMB_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(PWMB_COMPARE1_LSB_PTR, compare);
        #endif /* (PWMB_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: PWMB_WriteCompare2
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
    void PWMB_WriteCompare2(uint16 compare) \
                                        
    {
        #if(PWMB_UsingFixedFunction)
            CY_SET_REG16(PWMB_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG16(PWMB_COMPARE2_LSB_PTR, compare);
        #endif /* (PWMB_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (PWMB_DeadBandUsed)


    /*******************************************************************************
    * Function Name: PWMB_WriteDeadTime
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
    void PWMB_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!PWMB_DeadBand2_4)
            CY_SET_REG8(PWMB_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            PWMB_DEADBAND_COUNT &= ((uint8)(~PWMB_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(PWMB_DEADBAND_COUNT_SHIFT)
                PWMB_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << PWMB_DEADBAND_COUNT_SHIFT)) &
                                                    PWMB_DEADBAND_COUNT_MASK;
            #else
                PWMB_DEADBAND_COUNT |= deadtime & PWMB_DEADBAND_COUNT_MASK;
            #endif /* (PWMB_DEADBAND_COUNT_SHIFT) */

        #endif /* (!PWMB_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: PWMB_ReadDeadTime
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
    uint8 PWMB_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!PWMB_DeadBand2_4)
            return (CY_GET_REG8(PWMB_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(PWMB_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(PWMB_DEADBAND_COUNT & PWMB_DEADBAND_COUNT_MASK)) >>
                                                                           PWMB_DEADBAND_COUNT_SHIFT));
            #else
                return (PWMB_DEADBAND_COUNT & PWMB_DEADBAND_COUNT_MASK);
            #endif /* (PWMB_DEADBAND_COUNT_SHIFT) */
        #endif /* (!PWMB_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (PWMB_UseStatus || PWMB_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: PWMB_SetInterruptMode
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
    void PWMB_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(PWMB_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: PWMB_ReadStatusRegister
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
    uint8 PWMB_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(PWMB_STATUS_PTR));
    }

#endif /* (PWMB_UseStatus || PWMB_UsingFixedFunction) */


#if (PWMB_UseControl)


    /*******************************************************************************
    * Function Name: PWMB_ReadControlRegister
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
    uint8 PWMB_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(PWMB_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: PWMB_WriteControlRegister
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
    void PWMB_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(PWMB_CONTROL_PTR, control);
    }

#endif /* (PWMB_UseControl) */


#if (!PWMB_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: PWMB_ReadCapture
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
    uint16 PWMB_ReadCapture(void) 
    {
        return (CY_GET_REG16(PWMB_CAPTURE_LSB_PTR));
    }

#endif /* (!PWMB_UsingFixedFunction) */


#if (PWMB_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: PWMB_ReadCompare
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
    uint16 PWMB_ReadCompare(void) 
    {
        #if(PWMB_UsingFixedFunction)
            return ((uint16)CY_GET_REG16(PWMB_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG16(PWMB_COMPARE1_LSB_PTR));
        #endif /* (PWMB_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: PWMB_ReadCompare1
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
    uint16 PWMB_ReadCompare1(void) 
    {
        return (CY_GET_REG16(PWMB_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: PWMB_ReadCompare2
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
    uint16 PWMB_ReadCompare2(void) 
    {
        return (CY_GET_REG16(PWMB_COMPARE2_LSB_PTR));
    }

#endif /* (PWMB_UseOneCompareMode) */


/*******************************************************************************
* Function Name: PWMB_ReadPeriod
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
uint16 PWMB_ReadPeriod(void) 
{
    #if(PWMB_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(PWMB_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG16(PWMB_PERIOD_LSB_PTR));
    #endif /* (PWMB_UsingFixedFunction) */
}

#if ( PWMB_KillModeMinTime)


    /*******************************************************************************
    * Function Name: PWMB_WriteKillTime
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
    void PWMB_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(PWMB_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: PWMB_ReadKillTime
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
    uint8 PWMB_ReadKillTime(void) 
    {
        return (CY_GET_REG8(PWMB_KILLMODEMINTIME_PTR));
    }

#endif /* ( PWMB_KillModeMinTime) */

/* [] END OF FILE */
