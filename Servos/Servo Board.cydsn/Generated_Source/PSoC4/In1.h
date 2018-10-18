/*******************************************************************************
* File Name: In1.h  
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

#if !defined(CY_PINS_In1_H) /* Pins In1_H */
#define CY_PINS_In1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "In1_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    In1_Write(uint8 value) ;
void    In1_SetDriveMode(uint8 mode) ;
uint8   In1_ReadDataReg(void) ;
uint8   In1_Read(void) ;
uint8   In1_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define In1_DRIVE_MODE_BITS        (3)
#define In1_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - In1_DRIVE_MODE_BITS))

#define In1_DM_ALG_HIZ         (0x00u)
#define In1_DM_DIG_HIZ         (0x01u)
#define In1_DM_RES_UP          (0x02u)
#define In1_DM_RES_DWN         (0x03u)
#define In1_DM_OD_LO           (0x04u)
#define In1_DM_OD_HI           (0x05u)
#define In1_DM_STRONG          (0x06u)
#define In1_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define In1_MASK               In1__MASK
#define In1_SHIFT              In1__SHIFT
#define In1_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define In1_PS                     (* (reg32 *) In1__PS)
/* Port Configuration */
#define In1_PC                     (* (reg32 *) In1__PC)
/* Data Register */
#define In1_DR                     (* (reg32 *) In1__DR)
/* Input Buffer Disable Override */
#define In1_INP_DIS                (* (reg32 *) In1__PC2)


#if defined(In1__INTSTAT)  /* Interrupt Registers */

    #define In1_INTSTAT                (* (reg32 *) In1__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define In1_DRIVE_MODE_SHIFT       (0x00u)
#define In1_DRIVE_MODE_MASK        (0x07u << In1_DRIVE_MODE_SHIFT)


#endif /* End Pins In1_H */


/* [] END OF FILE */
