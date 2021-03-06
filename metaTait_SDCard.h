
#ifndef METATAIT_SDCARD_H_
#define METATAIT_SDCARD_H_
#include "F28x_Project.h"
#include "metaTait_HighLevel.h"


/* Definitions for MMC/SDC command */
#define CMD0    (0x40+0)    /* GO_IDLE_STATE */
#define CMD1    (0x40+1)    /* SEND_OP_COND */
#define CMD8    (0x40+8)    /* SEND_IF_COND */
#define CMD9    (0x40+9)    /* SEND_CSD */
#define CMD10    (0x40+10)    /* SEND_CID */
#define CMD12    (0x40+12)    /* STOP_TRANSMISSION */
#define CMD16    (0x40+16)    /* SET_BLOCKLEN */
#define CMD17    (0x40+17)    /* READ_SINGLE_BLOCK */
#define CMD18    (0x40+18)    /* READ_MULTIPLE_BLOCK */
#define CMD23    (0x40+23)    /* SET_BLOCK_COUNT */
#define CMD24    (0x40+24)    /* WRITE_BLOCK */
#define CMD25    (0x40+25)    /* WRITE_MULTIPLE_BLOCK */
#define CMD41    (0x40+41)    /* SEND_OP_COND (ACMD) */
#define CMD55    (0x40+55)    /* APP_CMD */
#define CMD58    (0x40+58)    /* READ_OCR */

typedef signed char     CHAR;
typedef unsigned char   UCHAR;
typedef unsigned char   BYTE;



/* Status of Disk Functions */
typedef BYTE    DSTATUS;

/* Results of Disk Functions */
typedef enum {
    RES_OK = 0,     /* 0: Successful */
    RES_ERROR,      /* 1: R/W Error */
    RES_WRPRT,      /* 2: Write Protected */
    RES_NOTRDY,     /* 3: Not Ready */
    RES_PARERR      /* 4: Invalid Parameter */
} DRESULT;

#define STA_NOINIT      0x01    /* Drive not initialized */
#define STA_NODISK      0x02    /* No medium in the drive */
#define STA_PROTECT     0x04    /* Write protected */

static volatile
DSTATUS Stat = STA_NOINIT;    /* Disk status */

static volatile
BYTE Timer1, Timer2;    /* 100Hz decrement timer */

static
BYTE CardType;            /* b0:MMC, b1:SDC, b2:Block addressing */

static
BYTE PowerFlag = 0;     /* indicates if "power" is on */
typedef enum { FALSE = 0, TRUE } BOOL;

/* These types are assumed as 8-bit integer */
typedef signed int      INT;
typedef unsigned int    UINT;


/* These types are assumed as 16-bit integer */
typedef signed short    SHORT;
typedef unsigned short  USHORT;
typedef unsigned short  WORD;

/* These types are assumed as 32-bit integer */
typedef signed long     LONG;
typedef unsigned long   ULONG;
typedef unsigned long   DWORD;



#define GET_SECTOR_COUNT	1
#define GET_SECTOR_SIZE		2
#define CTRL_SYNC			3
#define CTRL_POWER			4
#define CTRL_LOCK			5
#define CTRL_EJECT			6
#define MMC_GET_CSD			10
#define MMC_GET_CID			11
#define MMC_GET_OCR			12
#define ATA_GET_REV			20
#define ATA_GET_MODEL		21
#define ATA_GET_SN			22




/* Reading from SD Card via SPI Functions */
BYTE send_cmd(BYTE, DWORD);
BYTE rcvr_spi(void);
static void xmit_spi(BYTE);
BYTE wait_ready(void);
void spi_fifo_init(void);
void rcvr_spi_m (BYTE*);
void power_on (void);
DSTATUS disk_initialize(BYTE);
DSTATUS disk_status(BYTE);
DRESULT disk_read (BYTE, BYTE*, DWORD, BYTE);
void send_initial_clock_train (void);
static BOOL rcvr_datablock(BYTE*, UINT);
void disk_timerproc (void);




#endif /* METATAIT_SDCARD_H_ */
