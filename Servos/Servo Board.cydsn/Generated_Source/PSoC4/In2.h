/*******************************************************************************
* File Name: In2.h  
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

#if !defined(CY_PINS_In2_H) /* Pins In2_H */
#define CY_PINS_In2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "In2_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    In2_Write(uint8 value) ;
void    In2_SetDriveMode(uint8 mode) ;
uint8   In2_ReadDataReg(void) ;
uint8   In2_Read(void) ;
uint8   In2_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define In2_DRIVE_MODE_BITS        (3)
#define In2_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - In2_DRIVE_MODE_BITS))

#define In2_DM_ALG_HIZ         (0x00u)
#define In2_DM_DIG_HIZ         (0x01u)
#define In2_DM_RES_UP          (0x02u)
#define In2_DM_RES_DWN         (0x03u)
#define In2_DM_OD_LO           (0x04u)
#define In2_DM_OD_HI           (0x05u)
#define In2_DM_STRONG          (0x06u)
#define In2_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define In2_MASK               In2__MASK
#define In2_SHIFT              In2__SHIFT
#define In2_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define In2_PS                     (* (reg32 *) In2__PS)
/* Port Configuration */
#define In2_PC                     (* (reg32 *) In2__PC)
/* Data Register */
#define In2_DR                     (* (reg32 *) In2__DR)
/* Input Buffer Disable Override */
#define In2_INP_DIS                (* (reg32 *) In2__PC2)


#if defined(In2__INTSTAT)  /* Interrupt Registers */

    #define In2_INTSTAT                (* (reg32 *) In2__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define In2_DRIVE_MODE_SHIFT       (0x00u)
#define In2_DRIVE_MODE_MASK        (0x07u << In2_DRIVE_MODE_SHIFT)


#endif /* End Pins In2_H */


/* [] END OF FILE */
