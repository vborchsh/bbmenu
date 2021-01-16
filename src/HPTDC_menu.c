
#include "./inc/HPTDC_menu.h"

/**
 * @brief HPTDC_menu
 */
void HPTDC_menu(void)
{
	char *verb;
	int4 iverb;

    // Main cycle
    while(1)
	{
        verb = action_menu("HPTDC_menu", &iverb);
    
        if (strcmp(verb, "RETURN") == 0) {
            return;
        }

        if (strcmp(verb, "READ_FPGA_VER") == 0) {
            //GetFPGAver();
        }

        if (strcmp(verb, "HPTDC_WRITE_CFG") == 0) {
            //HPTDC_write_cfg();
        }

        if (strcmp(verb, "HPTDC_READ_CFG") == 0) {
            //HPTDC_read_cfg();
        }

        if (strcmp(verb, "HPTDC_READ_ID") == 0) {
            //HPTDC_read_ID();
        }

        if (strcmp(verb, "HPTDC_READ_STATUS") == 0) {
            //HPTDC_read_status();
        }

        if (strcmp(verb, "HPTDC_RESET") == 0) {
            //HPTDC_reset();
        }

        if (strcmp(verb, "SET_CTRL_MASK") == 0) {
            //ControlSel();
        }

        if (strcmp(verb, "SET_HPTDC") == 0) {
            //SetHPTDC();
        }

        if (strcmp(verb, "SET_JTAG") == 0) {
            //SetJTAGInstr(-1);
        }

        if (strcmp(verb, "SET_BOARD_ID") == 0) {
            //SetBoardID();
        }

        if (strcmp(verb, "SET_THRESHOLD") == 0) {
            //SetThreshold();
        }

        if (strcmp(verb, "SET_CONTROL_REG") == 0) {
            //SetCtrlReg();
        }

        if (strcmp(verb, "SET_PATTERN") == 0) {
            //SetPattern();
        }

        if (strcmp(verb, "DAQ") == 0) {
            //ReadHPTDCStream();
        }

        if (strcmp(verb, "FIFO_LVL") == 0) {
            //GetFIFOlevel();
        }
	}
}



