/*******************************************************************************
* File Name: CapSense_1_TunerHelper.h
* Version 3.50
*
* Description:
*  This file provides constants and structure declarations for the tunner hepl
*  APIs for CapSense CSD component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CAPSENSE_CSD_TUNERHELPER_CapSense_1_H)
#define CY_CAPSENSE_CSD_TUNERHELPER_CapSense_1_H

#include "CapSense_1.h"
#include "CapSense_1_CSHL.h"
#if (CapSense_1_TUNER_API_GENERATE)
    #include "CapSense_1_MBX.h"
    #include "EZI2C.h"
#endif /* (CapSense_1_TUNER_API_GENERATE) */


/***************************************
*     Constants for mailboxes
***************************************/

#define CapSense_1_DEFAULT_MAILBOXES_NUMBER   (1u)


/***************************************
*        Function Prototypes
***************************************/

void CapSense_1_TunerStart(void) ;
void CapSense_1_TunerComm(void) ;

#if (CapSense_1_TUNER_API_GENERATE)
    CapSense_1_NO_STRICT_VOLATILE void CapSense_1_ProcessAllWidgets(volatile CapSense_1_OUTBOX *outbox)
	                                        					;

    extern volatile CapSense_1_MAILBOXES CapSense_1_mailboxesComm;
#endif /* (CapSense_1_TUNER_API_GENERATE) */

#endif  /* (CY_CAPSENSE_CSD_TUNERHELPER_CapSense_1_H)*/


/* [] END OF FILE */
