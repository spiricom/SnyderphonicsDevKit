/* ========================================
 *
 * Copyright SNYDERPHONICS, 2019
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
    SNYDERPHONICS CAPACITIVE SENSING DEV KIT
 *
 * ========================================
*/
#include "project.h"


// create Max patch with documentation
// program two boards!
// ship those boards


//input params:  num sensors, capsense clock divider, sensor resolution, loop wait, 


#define DEVICE                  (0u)
#define MIDI_MSG_SIZE           (4u)

/*MIDI Message Fields */
#define MIDI_MSG_TYPE           (0u)
#define MIDI_CC_NUMBER        (1u)
#define MIDI_CC_VALUE      (2u)

#define USB_SUSPEND_TIMEOUT     (2u)

/*Logical Size of Em_EEPROM*/
#define LOGICAL_EEPROM_SIZE     15u
#define LOGICAL_EEPROM_START    0u

/*location of reset counter in Em_EEPROM*/
#define RESET_COUNT_LOCATION    13u
#define RESET_COUNT_SIZE        2u
/* EEPROM storage in work flash, this is defined in Em_EEPROM.c*/
const uint8_t Em_EEPROM_em_EepromStorage[Em_EEPROM_PHYSICAL_SIZE]
__ALIGNED(CY_FLASH_SIZEOF_ROW) = {0u};


cy_en_em_eeprom_status_t eepromReturnValue;


uint8_t EEPROMarray[15];

uint16_t capSensorsPrev[26];

void CapSense_DisplayState(void);
void setResolution(uint8_t newResolution);
void setIDAC(uint8_t newIDAC);
void  setActiveSensors(void);
void setClockDivider(uint8_t newClockDivider);
void setScanSpeed(uint8 newSpeed);

uint8_t numActiveSensors = 2;
uint8_t rawVsCooked = 0;
uint8_t maxActiveSensors = 26;
uint8_t clockDivider = 11;
uint8_t maxClockDivider = 255;
uint8_t minClockDivider = 1;
uint8_t delayBetweenLoops = 3;
uint8_t maxResolution = 16;
uint8_t minResolution = 8;
uint8_t maxIDAC = 255;
uint8_t minIDAC = 1;
uint8_t clockDividerLowByte = 0;
uint8_t IDACLowByte = 0;
uint8_t maxScanSpeed = 3;
uint8_t minScanSpeed = 0;
uint8_t sensorResolution = 10;

extern uint8 CapSense_widgetResolution[];
extern uint8 CapSense_idacSettings[];
extern uint8 CapSense_analogSwitchDivider;

volatile uint8 usbActivityCounter = 0u;

CY_ISR(SleepIsr)
{
    /* Check USB activity */
    if(0u != USB_CheckActivity()) 
    {
        usbActivityCounter = 0u;
    } 
    else 
    {
        usbActivityCounter++;
    }
    /* Clear Pending Interrupt */
    SleepTimer_GetStatus();
}


int main(void)
{
  
    uint8 midiMsg[MIDI_MSG_SIZE];   
            
    /* Enable global interrupts */
    CyGlobalIntEnable;
    Em_EEPROM_Init((uint32_t)Em_EEPROM_em_EepromStorage);

    /* Start USBFS device 0 with VDDD operation */
    USB_Start(DEVICE, USB_DWR_VDDD_OPERATION); 

    //maybe use the actual EEPROM in PSOC5 instead of emulated? 
    eepromReturnValue = Em_EEPROM_Read(LOGICAL_EEPROM_START, EEPROMarray, LOGICAL_EEPROM_SIZE); 
        
    //set local variables and settings to the settings retrieved from the EEPROM storage
    rawVsCooked = EEPROMarray[0];
    numActiveSensors = EEPROMarray[1];
    setActiveSensors();
    sensorResolution = EEPROMarray[2];
    setResolution(sensorResolution);
    setClockDivider(EEPROMarray[3]);
    setIDAC(EEPROMarray[4]);
    setScanSpeed( EEPROMarray[5]);
    
    
    CapSense_Start();
    CapSense_InitializeAllBaselines();
    
    while(1u)
    {
       
        /* Host can send double SET_INTERFACE request */
        if(0u != USB_IsConfigurationChanged())
        {
            /* Initialize IN endpoints when device configured */
            if(0u != USB_GetConfiguration())   
            {
                /* Start ISR to determine sleep condition */		
                Sleep_isr_StartEx(SleepIsr);
                
                /* Start SleepTimer's operation */
                SleepTimer_Start();
                
            	/* Enable output endpoint */
                USB_MIDI_Init();
            }
            else
            {
                SleepTimer_Stop();
            }    
        }        
        
        /* Check whether the scanning of all enabled widgets is completed. */
        if(0u == CapSense_IsBusy())
        {
            /* Update all baselines */
            CapSense_UpdateEnabledBaselines();

            /* Start scanning all enabled sensors */
            CapSense_ScanEnabledWidgets();
            
            
              /* Display CapSense state using LED */
            CapSense_DisplayState();
            
            
           
            
            /* Service USB MIDI when device is configured */
            if(0u != USB_GetConfiguration())    
            {
                /* Call this API from UART RX ISR for Auto DMA mode */
                #if(!USB_EP_MANAGEMENT_DMA_AUTO) 
                    USB_MIDI_IN_Service();
                #endif
                /* In Manual EP Memory Management mode OUT_EP_Service() 
                *  may have to be called from main foreground or from OUT EP ISR
                */
                #if(!USB_EP_MANAGEMENT_DMA_AUTO) 
                    USB_MIDI_OUT_Service();
                #endif

                
                for (int i = 0; i < numActiveSensors; i++)
                {
            		uint16_t tempSensor = 0;
                    
                    if (rawVsCooked == 0)
                    {
                        tempSensor = CapSense_sensorRaw[i];
                    }
                    else
                    {
                        tempSensor = CapSense_sensorSignal[i];
                    }
                    //filter out repeat values to keep the communication on the USB bus to a minimum
                    if (tempSensor != capSensorsPrev[i])
                    {   
                        midiMsg[MIDI_MSG_TYPE] = USB_MIDI_CONTROL_CHANGE;
            			midiMsg[MIDI_CC_NUMBER] = (i * 3);
                        midiMsg[MIDI_CC_VALUE] = (tempSensor & 127);
                        USB_PutUsbMidiIn(USB_3BYTE_COMMON, midiMsg, USB_MIDI_CABLE_00);                      
                        midiMsg[MIDI_MSG_TYPE] = USB_MIDI_CONTROL_CHANGE;
            			midiMsg[MIDI_CC_NUMBER] = (i * 3) + 1;
                        midiMsg[MIDI_CC_VALUE] =(tempSensor >> 7) & 127;
                        USB_PutUsbMidiIn(USB_3BYTE_COMMON, midiMsg, USB_MIDI_CABLE_00);
                        midiMsg[MIDI_CC_NUMBER] = (i * 3) + 2;
                        midiMsg[MIDI_CC_VALUE] =(tempSensor >> 14) & 127;
                        USB_PutUsbMidiIn(USB_3BYTE_COMMON, midiMsg, USB_MIDI_CABLE_00);
                    }
                    //store values for future comparison
                    capSensorsPrev[i] = tempSensor;
        		}
                
                            // Check if host requested USB Suspend 
                if( usbActivityCounter >= USB_SUSPEND_TIMEOUT ) 
                {
                    // Disable USBFS block and set DP Interrupt for wake-up 
                    // from sleep mode. 

                    USB_Suspend(); 
                    // Prepares system clocks for sleep mode 
                    CyPmSaveClocks();
                    
                    // Switch to the Sleep Mode for the PSoC 3 or PSoC 5LP devices:
                    //  - PM_SLEEP_TIME_NONE: wakeup time is defined by PICU source
                    //  - PM_SLEEP_SRC_PICU: PICU wakeup source 
                    
                    CyPmSleep(PM_SLEEP_TIME_NONE, PM_SLEEP_SRC_PICU);
                    // Restore clock configuration 
                    CyPmRestoreClocks();
                    // Enable USBFS block after power-down mode 
                    USB_Resume();
                    
                    // Enable output endpoint 
                    USB_MIDI_Init();

                    usbActivityCounter = 0u; // Re-init USB Activity Counter
                }
            }
        }
    }
}


void CapSense_DisplayState(void)
{
    /* Display BUTTON0 state */
    if (CapSense_CheckIsWidgetActive(CapSense_SENSOR_BUTTON0__BTN))
    {
        LED1_Write(1u);
    }
    else
    {
        LED1_Write(0u);
    }


}


/*******************************************************************************
* Function Name: USB_callbackLocalMidiEvent
********************************************************************************
* Summary: Local processing of the USB MIDI out-events.
*
*******************************************************************************/
void USB_callbackLocalMidiEvent(uint8 cable, uint8 *midiMsg) CYREENTRANT
{
    /* Support General System On/Off Message. */
    {
        if(midiMsg[USB_EVENT_BYTE0] == USB_MIDI_SYSEX_GEN_MESSAGE)
        {
            if(midiMsg[USB_EVENT_BYTE1] == USB_MIDI_SYSEX_SYSTEM_ON)
            {
                //MIDI_PWR_Write(0u); /* Power ON */
            }
            else if(midiMsg[USB_EVENT_BYTE1] == USB_MIDI_SYSEX_SYSTEM_OFF)
            {
                //MIDI_PWR_Write(1u); /* Power OFF */
            }
        }
        
        //raw vs cooked change
        if ((midiMsg[USB_EVENT_BYTE0] == USB_MIDI_NOTE_ON) && (midiMsg[USB_EVENT_BYTE1] == 61))
        {
            rawVsCooked = (midiMsg[USB_EVENT_BYTE2] > 0);
            EEPROMarray[0] = rawVsCooked;
            eepromReturnValue = Em_EEPROM_Write(LOGICAL_EEPROM_START, EEPROMarray, LOGICAL_EEPROM_SIZE);
        }
        
        
        //numActiveSensors change
        if ((midiMsg[USB_EVENT_BYTE0] == USB_MIDI_CONTROL_CHANGE) && (midiMsg[USB_EVENT_BYTE1] == 0))
        {
            if (midiMsg[USB_EVENT_BYTE2] <= maxActiveSensors)
            {
                numActiveSensors = midiMsg[USB_EVENT_BYTE2];
                
                EEPROMarray[1] = numActiveSensors;
                eepromReturnValue = Em_EEPROM_Write(LOGICAL_EEPROM_START, EEPROMarray, LOGICAL_EEPROM_SIZE);
                
                setActiveSensors();
            }
        }
        
        //set resolution
        if ((midiMsg[USB_EVENT_BYTE0] == USB_MIDI_CONTROL_CHANGE) && (midiMsg[USB_EVENT_BYTE1] == 1))
        {
            if ((midiMsg[USB_EVENT_BYTE2] <= maxResolution) && (midiMsg[USB_EVENT_BYTE2] >= minResolution))
            {
                sensorResolution = midiMsg[USB_EVENT_BYTE2];
                setResolution(sensorResolution);
                EEPROMarray[2] = sensorResolution;
                eepromReturnValue = Em_EEPROM_Write(LOGICAL_EEPROM_START, EEPROMarray, LOGICAL_EEPROM_SIZE);
            }
        }
       
        //clock divider high = 2
        //clock divider low = 3
        //capsense clock divider change
        
        if ((midiMsg[USB_EVENT_BYTE0] == USB_MIDI_CONTROL_CHANGE) && (midiMsg[USB_EVENT_BYTE1] == 2))
        {
            uint8_t tempClockDivider = (midiMsg[USB_EVENT_BYTE2] << 7) + clockDividerLowByte;

            if ((tempClockDivider <= maxClockDivider) && (tempClockDivider >= minClockDivider))
            {
                clockDivider = tempClockDivider;
                EEPROMarray[3] = clockDivider;
                eepromReturnValue = Em_EEPROM_Write(LOGICAL_EEPROM_START, EEPROMarray, LOGICAL_EEPROM_SIZE);
                setClockDivider(clockDivider);
            }
        }
        if ((midiMsg[USB_EVENT_BYTE0] == USB_MIDI_CONTROL_CHANGE) && (midiMsg[USB_EVENT_BYTE1] == 3))
        {
            if (midiMsg[USB_EVENT_BYTE2] < 128)
            {
                clockDividerLowByte = midiMsg[USB_EVENT_BYTE2];
            }
        }
        
        
        //IDAC high = 4
        //IDAC low = 5
        //set IDAC
        if ((midiMsg[USB_EVENT_BYTE0] == USB_MIDI_CONTROL_CHANGE) && (midiMsg[USB_EVENT_BYTE1] == 4))
        {
            uint8_t tempIDAC =  (midiMsg[USB_EVENT_BYTE2] << 7) + IDACLowByte;
            
            if ((tempIDAC <= maxIDAC) && (tempIDAC >= minIDAC))
            {
                setIDAC(tempIDAC);
                EEPROMarray[4] = tempIDAC;
                eepromReturnValue = Em_EEPROM_Write(LOGICAL_EEPROM_START, EEPROMarray, LOGICAL_EEPROM_SIZE);
            }
        }
        if ((midiMsg[USB_EVENT_BYTE0] == USB_MIDI_CONTROL_CHANGE) && (midiMsg[USB_EVENT_BYTE1] == 5))
        {
            if (midiMsg[USB_EVENT_BYTE2] < 128)
            {
                IDACLowByte = midiMsg[USB_EVENT_BYTE2];
            }
        }
        
        
        //set Scan Speed
        if ((midiMsg[USB_EVENT_BYTE0] == USB_MIDI_CONTROL_CHANGE) && (midiMsg[USB_EVENT_BYTE1] == 6))
        {
            uint8_t tempScanSpeed = midiMsg[USB_EVENT_BYTE2];
            if ((tempScanSpeed >= minScanSpeed) && (tempScanSpeed <= maxScanSpeed))
            {
                setScanSpeed(tempScanSpeed);
                EEPROMarray[5] = tempScanSpeed;
                eepromReturnValue = Em_EEPROM_Write(LOGICAL_EEPROM_START, EEPROMarray, LOGICAL_EEPROM_SIZE);
            }
        }
        
        //initialize baselines
        if ((midiMsg[USB_EVENT_BYTE0] == USB_MIDI_NOTE_ON) && (midiMsg[USB_EVENT_BYTE1] == 60))
        {
            CapSense_InitializeAllBaselines();
        }
        
        //initialize baselines
        CapSense_InitializeAllBaselines();
        
    }
    cable = cable;
}    

void setActiveSensors(void)
{
    // turn on the sensors before the numActiveSensors point
    for (int i = 0; i < numActiveSensors; i++)
    {
        CapSense_EnableWidget(i);
    }
    if (numActiveSensors < maxActiveSensors)
    {
       //turn off the sensors beyond the numActiveSensors point
        for(int i = numActiveSensors; i < maxActiveSensors; i++)
        {
            CapSense_DisableWidget(i);
        }
    }
}

void setScanSpeed(uint8 newSpeed)
{           
    if ((newSpeed <= maxScanSpeed) && (newSpeed >= minScanSpeed))
    {
        if (newSpeed == 0)
        {
            CapSense_SetScanSpeed(CapSense_SCAN_SPEED_SLOW);
        }
        else if (newSpeed == 1)
        {
            CapSense_SetScanSpeed(CapSense_SCAN_SPEED_NORMAL );
        }
        else if (newSpeed == 2)
        {
            CapSense_SetScanSpeed(CapSense_SCAN_SPEED_FAST  );
        }
        else if (newSpeed == 3)
        {
            CapSense_SetScanSpeed(CapSense_SCAN_SPEED_ULTRA_FAST );
        }
    }
}

void setResolution(uint8_t newResolution)
{
    uint8_t tempRes = 0;
    
    if (newResolution == 8)
    {
       tempRes = CapSense_PWM_RESOLUTION_8_BITS;
    }
    else if (newResolution == 9)
    {
         tempRes = CapSense_PWM_RESOLUTION_9_BITS;
    }
    else if (newResolution == 10)
    {
         tempRes = CapSense_PWM_RESOLUTION_10_BITS;
    }
    else if (newResolution == 11)
    {
         tempRes = CapSense_PWM_RESOLUTION_11_BITS;
    }
    else if (newResolution == 12)
    {
         tempRes = CapSense_PWM_RESOLUTION_12_BITS;
    }
    else if (newResolution == 13)
    {
         tempRes = CapSense_PWM_RESOLUTION_13_BITS;
    }
    else if (newResolution == 14)
    {
         tempRes = CapSense_PWM_RESOLUTION_14_BITS;
    }
    else if (newResolution == 15)
    {
         tempRes = CapSense_PWM_RESOLUTION_15_BITS;
    }
    else if (newResolution == 16)
    {
         tempRes = CapSense_PWM_RESOLUTION_16_BITS;
    }
    
    for (int i = 0; i < maxActiveSensors; i++)
    {
        CapSense_widgetResolution[i] = tempRes;
    }
}

void setClockDivider(uint8_t newClockDivider)
{
    CapSense_analogSwitchDivider = newClockDivider;
}

void setIDAC(uint8_t newIDAC)
{
    for (int i = 0; i < maxActiveSensors; i++)
    {
        CapSense_idacSettings[i] = newIDAC;
    }
}



/* [] END OF FILE */
