#ifndef __HPTDC_MENU__
#define __HPTDC_MENU__

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "./inc/BBmenuDefs.h"
#include "./inc/parameters.h"

// Parameters
#define pGOH_FRAMESIZE 2*192 /* bytes */

/*
 * Function prototypes
 */
void           HPTDC_menu(void);

void           ControlSel(void);
void           SetHPTDC(void);
void           HPTDC_write_cfg(void);
void           HPTDC_read_cfg(void);
void           HPTDC_read_ID(void);
void           HPTDC_read_status(void);
void           HPTDC_reset(void);
void           ReadHPTDCStream(void);
void           FlushFIFO(void);
short          GetFIFOlevel(void);
void           GetFPGAver(void);
short          SetJTAGInstr(short instr);
void           SetBoardID(void);
void           SetThreshold(void);
void           SetCtrlReg(void);
void           SetPattern(void);

void           data_process(FILE *pf, int frame_num, unsigned short data[][32]);

#endif
