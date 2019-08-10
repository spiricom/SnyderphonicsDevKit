/*******************************************************************************
* File Name: CapSense_Pins.h
* Version 3.50
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CAPSENSE_CSD_Pins_CapSense_H)
#define CY_CAPSENSE_CSD_Pins_CapSense_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "CapSense.h"


/***************************************
*        Function Prototypes
***************************************/

void CapSense_SetAllSensorsDriveMode(uint8 mode) ;
void CapSense_SetAllCmodsDriveMode(uint8 mode) ;
#if (CapSense_CURRENT_SOURCE == CapSense_EXTERNAL_RB)
    void CapSense_SetAllRbsDriveMode(uint8 mode) ;    
#endif  /* (CapSense_CURRENT_SOURCE == CapSense_EXTERNAL_RB) */   


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define CapSense_DM_ALG_HIZ         (PIN_DM_ALG_HIZ)
#define CapSense_DM_DIG_HIZ         (PIN_DM_DIG_HIZ)
#define CapSense_DM_RES_UP          (PIN_DM_RES_UP)
#define CapSense_DM_RES_DWN         (PIN_DM_RES_DWN)
#define CapSense_DM_OD_LO           (PIN_DM_OD_LO)
#define CapSense_DM_OD_HI           (PIN_DM_OD_HI)
#define CapSense_DM_STRONG          (PIN_DM_STRONG)
#define CapSense_DM_RES_UPDWN       (PIN_DM_RES_UPDWN)

/* PC registers defines for sensors */
#define CapSense_PortCH0__Button0__BTN  CapSense_PortCH0__Button0__BTN__PC
#define CapSense_PortCH0__Button1__BTN  CapSense_PortCH0__Button1__BTN__PC
#define CapSense_PortCH0__Button2__BTN  CapSense_PortCH0__Button2__BTN__PC
#define CapSense_PortCH0__Button3__BTN  CapSense_PortCH0__Button3__BTN__PC
#define CapSense_PortCH0__Button4__BTN  CapSense_PortCH0__Button4__BTN__PC
#define CapSense_PortCH0__Button5__BTN  CapSense_PortCH0__Button5__BTN__PC
#define CapSense_PortCH0__Button6__BTN  CapSense_PortCH0__Button6__BTN__PC
#define CapSense_PortCH0__Button7__BTN  CapSense_PortCH0__Button7__BTN__PC
#define CapSense_PortCH0__Button8__BTN  CapSense_PortCH0__Button8__BTN__PC
#define CapSense_PortCH0__Button9__BTN  CapSense_PortCH0__Button9__BTN__PC
#define CapSense_PortCH0__Button10__BTN  CapSense_PortCH0__Button10__BTN__PC
#define CapSense_PortCH0__Button11__BTN  CapSense_PortCH0__Button11__BTN__PC
#define CapSense_PortCH0__Button12__BTN  CapSense_PortCH0__Button12__BTN__PC
#define CapSense_PortCH0__Button13__BTN  CapSense_PortCH0__Button13__BTN__PC
#define CapSense_PortCH0__Button14__BTN  CapSense_PortCH0__Button14__BTN__PC
#define CapSense_PortCH0__Button15__BTN  CapSense_PortCH0__Button15__BTN__PC
#define CapSense_PortCH0__Button16__BTN  CapSense_PortCH0__Button16__BTN__PC
#define CapSense_PortCH0__Button17__BTN  CapSense_PortCH0__Button17__BTN__PC
#define CapSense_PortCH0__Button18__BTN  CapSense_PortCH0__Button18__BTN__PC
#define CapSense_PortCH0__Button19__BTN  CapSense_PortCH0__Button19__BTN__PC
#define CapSense_PortCH0__Button20__BTN  CapSense_PortCH0__Button20__BTN__PC
#define CapSense_PortCH0__Button21__BTN  CapSense_PortCH0__Button21__BTN__PC
#define CapSense_PortCH0__Button22__BTN  CapSense_PortCH0__Button22__BTN__PC
#define CapSense_PortCH0__Button23__BTN  CapSense_PortCH0__Button23__BTN__PC
#define CapSense_PortCH0__Button24__BTN  CapSense_PortCH0__Button24__BTN__PC
#define CapSense_PortCH0__Button25__BTN  CapSense_PortCH0__Button25__BTN__PC
/* For Cmods*/
#define CapSense_CmodCH0_Cmod_CH0       CapSense_CmodCH0__Cmod_CH0__PC


#endif /* (CY_CAPSENSE_CSD_Pins_CapSense_H) */


/* [] END OF FILE */
