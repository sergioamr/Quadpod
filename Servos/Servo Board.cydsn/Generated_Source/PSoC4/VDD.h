/*******************************************************************************
* File Name: VDD.h  
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

#if !defined(CY_PINS_VDD_H) /* Pins VDD_H */
#define CY_PINS_VDD_H

#include "cytypes.h"
#include "cyfitter.h"
#include "VDD_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    VDD_Write(uint8 value) ;
void    VDD_SetDriveMode(uint8 mode) ;
uint8   VDD_ReadDataReg(void) ;
uint8   VDD_Read(void) ;
uint8   VDD_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define VDD_DRIVE_MODE_BITS        (3)
#define VDD_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - VDD_DRIVE_MODE_BITS))

#define VDD_DM_ALG_HIZ         (0x00u)
#define VDD_DM_DIG_HIZ         (0x01u)
#define VDD_DM_RES_UP          (0x02u)
#define VDD_DM_RES_DWN         (0x03u)
#define VDD_DM_OD_LO           (0x04u)
#define VDD_DM_OD_HI           (0x05u)
#define VDD_DM_STRONG          (0x06u)
#define VDD_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define VDD_MASK               VDD__MASK
#define VDD_SHIFT              VDD__SHIFT
#define VDD_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define VDD_PS                     (* (reg32 *) VDD__PS)
/* Port Configuration */
#define VDD_PC                     (* (reg32 *) VDD__PC)
/* Data Register */
#define VDD_DR                     (* (reg32 *) VDD__DR)
/* Input Buffer Disable Override */
#define VDD_INP_DIS                (* (reg32 *) VDD__PC2)


#if defined(VDD__INTSTAT)  /* Interrupt Registers */

    #define VDD_INTSTAT                (* (reg32 *) VDD__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define VDD_DRIVE_MODE_SHIFT       (0x00u)
#define VDD_DRIVE_MODE_MASK        (0x07u << VDD_DRIVE_MODE_SHIFT)


#endif /* End Pins VDD_H */


/* [] END OF FILE */
