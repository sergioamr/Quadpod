/*******************************************************************************
* File Name: MotorIn1.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_MotorIn1_H) /* Pins MotorIn1_H */
#define CY_PINS_MotorIn1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "MotorIn1_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    MotorIn1_Write(uint8 value) ;
void    MotorIn1_SetDriveMode(uint8 mode) ;
uint8   MotorIn1_ReadDataReg(void) ;
uint8   MotorIn1_Read(void) ;
uint8   MotorIn1_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define MotorIn1_DRIVE_MODE_BITS        (3)
#define MotorIn1_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - MotorIn1_DRIVE_MODE_BITS))

#define MotorIn1_DM_ALG_HIZ         (0x00u)
#define MotorIn1_DM_DIG_HIZ         (0x01u)
#define MotorIn1_DM_RES_UP          (0x02u)
#define MotorIn1_DM_RES_DWN         (0x03u)
#define MotorIn1_DM_OD_LO           (0x04u)
#define MotorIn1_DM_OD_HI           (0x05u)
#define MotorIn1_DM_STRONG          (0x06u)
#define MotorIn1_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define MotorIn1_MASK               MotorIn1__MASK
#define MotorIn1_SHIFT              MotorIn1__SHIFT
#define MotorIn1_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define MotorIn1_PS                     (* (reg32 *) MotorIn1__PS)
/* Port Configuration */
#define MotorIn1_PC                     (* (reg32 *) MotorIn1__PC)
/* Data Register */
#define MotorIn1_DR                     (* (reg32 *) MotorIn1__DR)
/* Input Buffer Disable Override */
#define MotorIn1_INP_DIS                (* (reg32 *) MotorIn1__PC2)


#if defined(MotorIn1__INTSTAT)  /* Interrupt Registers */

    #define MotorIn1_INTSTAT                (* (reg32 *) MotorIn1__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define MotorIn1_DRIVE_MODE_SHIFT       (0x00u)
#define MotorIn1_DRIVE_MODE_MASK        (0x07u << MotorIn1_DRIVE_MODE_SHIFT)


#endif /* End Pins MotorIn1_H */


/* [] END OF FILE */
