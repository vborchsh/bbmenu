#ifndef PARAMETERS_H
#define PARAMETERS_H

#define pAPP_VERSION 0.9

// I2C address map --------------------------------------------
// R/W addresses
#define pADR_JTAG_INSTR 0x00
#define pADR_HPTDC_CFG  0x51
#define pADR_BOARD_ID   0x52
#define pADR_THRESHOLD  0x53
#define pADR_CTRL_REG   0x54
#define pADR_PTRN_SLCT  0x55
#define pADR_MUX_CTRL   0x64
// R/O addresses
#define pADR_MUX_DATA   0x70
#define pADR_FW_TOP     0x7C /* fw top ver    */
#define pADR_FW_VER     0x7D /* fw version    */
#define pADR_FW_SUB     0x7E /* fw subversion */
#define pADR_FW_REV     0x7F /* fw revision   */

// Command address map --------------------------------------------
#define pCMD_I2C_MUX    0x00
#define pCMD_I2C_SRC    0x01
// RO
#define pCMD_FIFO_LVL   0x80

// JTAG address map --------------------------------------------
#define pJTAG_CONFIG     0x00
#define pJTAG_STATUS     0x02
#define pJTAG_ID         0x03
#define pJTAG_RST_HPTDC  0x08

typedef struct {
    int        bunchnum;
    int        eventnum;
    int        threshold;
    int        leading;
    int        trailing;
} channel_t;


/* a=target variable, b=bit number to act upon 0-n */
#define BIT_SET(a,b)   ((a) |= (1ULL<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1ULL<<(b)))

#endif // PARAMETERS_H
