#ifndef __QUSB_MENU__
#define __QUSB_MENU__

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "QuickUSB.h"
#include "BBmenuDefs.h"

/*
 * Function prototypes
 */
void            QuickUSB_menu(void);

uint2           Find_Modules(void);
uint2           Close_Device(void);
void            QUSB_LastError(void);
void            RSettings(void);
void            WSettings(void);
void            RDefault(void);
void            WDefault(void);
void            I2C_Read();
void            I2C_Write();
void            Command_Read();
void            Command_Write();
void            Data_Read();
void            Port_RDir();
void            Port_WDir();
void            Port_Read();
void            Port_Write();

#endif
