/*******************************************************************************
* File Name: MotorIn2.h  
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

#if !defined(CY_PINS_MotorIn2_H) /* Pins MotorIn2_H */
#define CY_PINS_MotorIn2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "MotorIn2_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    MotorIn2_Write(uint8 value) ;
void    MotorIn2_SetDriveMode(uint8 mode) ;
uint8   MotorIn2_ReadDataReg(void) ;
uint8   MotorIn2_Read(void) ;
uint8   MotorIn2_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define MotorIn2_DRIVE_MODE_BITS        (3)
#define MotorIn2_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - MotorIn2_DRIVE_MODE_BITS))

#define MotorIn2_DM_ALG_HIZ         (0x00u)
#define MotorIn2_DM_DIG_HIZ         (0x01u)
#define MotorIn2_DM_RES_UP          (0x02u)
#define MotorIn2_DM_RES_DWN         (0x03u)
#define MotorIn2_DM_OD_LO           (0x04u)
#define MotorIn2_DM_OD_HI           (0x05u)
#define MotorIn2_DM_STRONG          (0x06u)
#define MotorIn2_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define MotorIn2_MASK               MotorIn2__MASK
#define MotorIn2_SHIFT              MotorIn2__SHIFT
#define MotorIn2_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define MotorIn2_PS                     (* (reg32 *) MotorIn2__PS)
/* Port Configuration */
#define MotorIn2_PC                     (* (reg32 *) MotorIn2__PC)
/* Data Register */
#define MotorIn2_DR                     (* (reg32 *) MotorIn2__DR)
/* Input Buffer Disable Override */
#define MotorIn2_INP_DIS                (* (reg32 *) MotorIn2__PC2)


#if defined(MotorIn2__INTSTAT)  /* Interrupt Registers */

    #define MotorIn2_INTSTAT                (* (reg32 *) MotorIn2__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define MotorIn2_DRIVE_MODE_SHIFT       (0x00u)
#define MotorIn2_DRIVE_MODE_MASK        (0x07u << MotorIn2_DRIVE_MODE_SHIFT)


#endif /* End Pins MotorIn2_H */


/* [] END OF FILE */
