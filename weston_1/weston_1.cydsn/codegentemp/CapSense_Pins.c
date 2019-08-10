/*******************************************************************************
* File Name: CapSense_Pins.c
* Version 3.50
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "CapSense_Pins.h"


/*******************************************************************************
* Function Name: CapSense_SetAllSensorsDriveMode
********************************************************************************
*
* Summary:
*  Sets the drive mode for the all pins used by capacitive sensors within 
*  CapSense component.
* 
* Parameters:  
*  mode: Desired drive mode.
*
* Return: 
*  None
*
*******************************************************************************/
void CapSense_SetAllSensorsDriveMode(uint8 mode) 
{
    /* Set pins drive mode */
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button0__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button1__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button2__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button3__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button4__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button5__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button6__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button7__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button8__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button9__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button10__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button11__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button12__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button13__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button14__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button15__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button16__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button17__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button18__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button19__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button20__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button21__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button22__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button23__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button24__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_PortCH0__Button25__BTN, mode);

}



/*******************************************************************************
* Function Name: CapSense_SetAllCmodsDriveMode
********************************************************************************
*
* Summary:
*  Sets the drive mode for the all pins used by Cmod capacitors within CapSense 
*  component.
* 
* Parameters:  
*  mode: Desired drive mode.
*
* Return: 
*  None
*
*******************************************************************************/
void CapSense_SetAllCmodsDriveMode(uint8 mode) 
{
   /* Set pins drive mode */
    CyPins_SetPinDriveMode(CapSense_CmodCH0_Cmod_CH0, mode);

}


#if (CapSense_CURRENT_SOURCE == CapSense_EXTERNAL_RB)
    /*******************************************************************************
    * Function Name: CapSense_SetAllRbsDriveMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the drive mode for the all pins used by bleed resistors (Rb) within 
    *  CapSense component. Only available when Current Source is external resistor.
    * 
    * Parameters:  
    *  mode: Desired drive mode.
    *
    * Return: 
    *  None
    *
    *******************************************************************************/
    void CapSense_SetAllRbsDriveMode(uint8 mode) 
    {
        /* Set pins drive mode */
      
    }
#endif  /* (CapSense_CURRENT_SOURCE == CapSense_EXTERNAL_RB) */


/* [] END OF FILE */
