
#include "inc/QuickUSB_menu.h"

extern QHANDLE hDevice;

void QuickUSB_menu(void)
{
  char *verb;
  int4 iverb;
  while(1)
  {
    verb = action_menu("QuickUSB_menu", &iverb);
    if(strcmp(verb,"FIND_MODULES"   )==0) 		;//Find_Modules();
    if(strcmp(verb,"CLOSE_DEVICE"   )==0) 		;//Close_Device();
    if(strcmp(verb,"LAST_ERROR"		)==0)		;//QUSB_LastError();
    if(strcmp(verb,"RSETTINGS"		)==0) 		;//RSettings();
    if(strcmp(verb,"WSETTINGS"		)==0) 		;//WSettings();
    if(strcmp(verb,"RDEFAULT"       )==0) 		;//RDefault();
    if(strcmp(verb,"WDEFAULT"       )==0) 		;//WDefault();
    if(strcmp(verb,"I2C_READ"		)==0)		;//I2C_Read();
    if(strcmp(verb,"I2C_WRITE"		)==0)		;//I2C_Write();
    if(strcmp(verb,"COMMAND_READ"	)==0) 		;//Command_Read();
    if(strcmp(verb,"COMMAND_WRITE"	)==0)		;//Command_Write();
    if(strcmp(verb,"DATA_READ"		)==0)		;//Data_Read();
    if(strcmp(verb,"PORT_RDIR"		)==0)		;//Port_RDir();
    if(strcmp(verb,"PORT_WDIR"		)==0)		;//Port_WDir();
    if(strcmp(verb,"PORT_READ"		)==0)		;//Port_Read();
    if(strcmp(verb,"PORT_WRITE"		)==0)		;//Port_Write();
    if(strcmp(verb,"RETURN"         )==0)		return;
	}
}


	
	
	
	
	
	
	
	
	
