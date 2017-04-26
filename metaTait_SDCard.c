#include "F28x_Project.h"
#include "metaTait_SPI.h"
#include "metaTait_McBSP.h"
#include "metaTait_SDCard.h"


int i = 0;

void send_initial_clock_train (void)        //Change to whichever pins we are using.
{
    volatile unsigned long dat;

    /* Ensure CS (STE) is held high. */
    GpioDataRegs.GPASET.bit.GPIO19 = 1;

    /* Switch the SPI TX line to a GPIO and drive it high too. */
    EALLOW;
    GpioCtrlRegs.GPALOCK.bit.GPIO16 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO16 = 1;
    GpioCtrlRegs.GPALOCK.bit.GPIO16 = 1;
    EDIS;

    GpioDataRegs.GPASET.bit.GPIO16 = 1;        //Drive TX High

    /* Send 10 bytes over the SPI. This causes the clock to wiggle the */
    /* required number of times. */
    while(i < 10)
    {
        /* Write DUMMY data */
        while(SpiaRegs.SPISTS.bit.BUFFULL_FLAG);
        SpiaRegs.SPITXBUF = 0xFF00;

        /* Flush data read during data write. */
        while(SpiaRegs.SPISTS.bit.INT_FLAG !=1);        //May be possible to switch to '!SpicRegs.SPISTS.bit.INT_FLAG'
        dat = (SpiaRegs.SPIRXBUF & 0xFF);
        i++;
    }
    /* Revert to hardware control of the SPI TX line. */
    EALLOW;
    GpioCtrlRegs.GPALOCK.bit.GPIO16 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 2;
    GpioCtrlRegs.GPALOCK.bit.GPIO16 = 1;
    EDIS;
}

/*-----------------------------------------------------------------------*/
/* Send a command packet to MMC                                          */
/*-----------------------------------------------------------------------*/

BYTE send_cmd (
    BYTE cmd,        /* Command byte */
    DWORD arg        /* Argument */
)
{
    BYTE n, res;



    if (wait_ready() != 0xFF) return 0xFF;

    /* Send command packet */
    xmit_spi(cmd);                        /* Command */
    xmit_spi((BYTE)(arg >> 24));        /* Argument[31..24] */
    xmit_spi((BYTE)(arg >> 16));        /* Argument[23..16] */
    xmit_spi((BYTE)(arg >> 8));            /* Argument[15..8] */
    xmit_spi((BYTE)arg);                /* Argument[7..0] */
    n = 0;
    if (cmd == CMD0) n = 0x95;            /* CRC for CMD0(0) */
    if (cmd == CMD8) n = 0x87;            /* CRC for CMD8(0x1AA) */
    xmit_spi(n);

    /* Receive command response */
    if (cmd == CMD12) rcvr_spi();        /* Skip a stuff byte when stop reading */
    n = 10;                                /* Wait for a valid response in timeout of 10 attempts */
    res = rcvr_spi();
    while ((res & 0x80) && --n)
    {
        res = rcvr_spi();
    }
    return res;            /* Return with the response value */
}




BYTE wait_ready(void)
{

    unsigned char res;
    Timer2 = 50;    /* Wait for ready in timeout of 500ms */
    rcvr_spi();
    res = rcvr_spi();
    do
    {
        res = rcvr_spi();
        DELAY_US(175);
    }while((res != 0xFF) && Timer2);
    return res;
}


/*-----------------------------------------------------------------------*/
/* Transmit a byte to MMC via SPI  (Platform dependent)                  */
/*-----------------------------------------------------------------------*/

static
void xmit_spi (BYTE dat)    //EDITED
{
    volatile DWORD rcvdat;                      //included file like integer.h for DWORD definition

    /* Write the data to the tx fifo */
    while(SpiaRegs.SPISTS.bit.BUFFULL_FLAG);    //Wait for room to write data
    SpiaRegs.SPITXBUF = ((DWORD)dat)<<8;                        //Write data

    /* flush data read during the write */
    while(SpiaRegs.SPISTS.bit.INT_FLAG !=1);        //May be possible to switch to '!SpicRegs.SPISTS.bit.INT_FLAG'
    rcvdat = (SpiaRegs.SPIRXBUF && 0xFF);                   //Clear Receive Buffer
}


/*-----------------------------------------------------------------------*/
/* Receive a byte from MMC via SPI  (Platform dependent)                 */
/*-----------------------------------------------------------------------*/

BYTE rcvr_spi (void)        //EDITED
{
    volatile DWORD rcvdat;

    //Disable transmission channel
    //SpicRegs.SPICTL.bit.TALK = 0;

    /* write dummy data */
    while(SpiaRegs.SPISTS.bit.BUFFULL_FLAG);    //Wait for space to write
    SpiaRegs.SPITXBUF = 0xFF00;                     //Write dummy data

    /* read data from RX fifo */
    while(SpiaRegs.SPISTS.bit.INT_FLAG !=1);        //May be possible to switch to '!SpicRegs.SPISTS.bit.INT_FLAG'
    rcvdat = (SpiaRegs.SPIRXBUF & 0xFF);                    //Read Transferred data

    return (BYTE)rcvdat;
}


void rcvr_spi_m (BYTE *dst)
{
    *dst = rcvr_spi();
}


void power_on (void)    //Configure to actual pins
{
    /*
     * This doesn't really turn the power on, but initializes the
     * SPI port and pins needed to talk to the SD card.
     */
    EALLOW;
    /* Enable the peripherals used to drive the SDC on SPI */
    CpuSysRegs.PCLKCR8.bit.SPI_C = 1;

    /*
     * Configure the appropriate pins to be SPI instead of GPIO. The CS
     * signal is directly driven to ensure that we can hold it low through a
     * complete transaction with the SD card.
     */


    //122 - 16
    //123 - 17
    //124 - 18
    //125 - 19
    //Unlock the SD-Card SPI GPIOs
    GpioCtrlRegs.GPALOCK.bit.GPIO16 = 0;        //MOSI  GPIO16 = Pin 36
    GpioCtrlRegs.GPALOCK.bit.GPIO17 = 0;        //MISO	GPIO17 = Pin 35
    GpioCtrlRegs.GPALOCK.bit.GPIO18 = 0;        //SCLK	GPIO18 = Pin 76
    GpioCtrlRegs.GPALOCK.bit.GPIO19 = 0;        //STEA	GPIO19 = Pin 75

    //Set up  MUX & DIR
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0;   //Leave as GPIO for manual CS control

    GpioCtrlRegs.GPAGMUX2.bit.GPIO16 = 1;
    GpioCtrlRegs.GPAGMUX2.bit.GPIO17 = 1;
    GpioCtrlRegs.GPAGMUX2.bit.GPIO18 = 1;

    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 2;
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 2;
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;

    GpioCtrlRegs.GPADIR.bit.GPIO19 = 1;

    //Set up GPIO Pull-up disables/enables
    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;    //Needs to be normally pulled high
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;    //Needs to be normally pulled high
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 1;

    //Set up GPIOs in asynch mode
    GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3; // Asynch input
    GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;

    //Configure GPIOs for CPU1
    GpioCtrlRegs.GPACSEL3.bit.GPIO16 = 0;
    GpioCtrlRegs.GPACSEL3.bit.GPIO17 = 0;
    GpioCtrlRegs.GPACSEL3.bit.GPIO18 = 0;
    GpioCtrlRegs.GPACSEL3.bit.GPIO19 = 0;

    //Lock the SD-Card SPI GPIOs
    GpioCtrlRegs.GPALOCK.bit.GPIO16 = 1;
    GpioCtrlRegs.GPALOCK.bit.GPIO17 = 1;
    GpioCtrlRegs.GPALOCK.bit.GPIO18 = 1;
    GpioCtrlRegs.GPALOCK.bit.GPIO19 = 1;
    EDIS;


    /* Deassert the SPIA chip selects for both the SD card and serial flash */
    GpioDataRegs.GPASET.bit.GPIO19 = 1;

    /* Configure the SPI A port */
    SpiaRegs.SPICCR.bit.SPISWRESET = 0;         //Set reset bit low
    SpiaRegs.SPICTL.bit.CLK_PHASE = 0;
    SpiaRegs.SPICCR.bit.CLKPOLARITY = 1;
    SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1;       //Master mode
    SpiaRegs.SPIBRR.all = 63;                       //update value to proper setting for correct bitrate ( current: ~500kHz)
    SpiaRegs.SPICCR.bit.SPICHAR = 0x7;          //Set char length to 8 bits
    SpiaRegs.SPICTL.bit.TALK = 1;
    SpiaRegs.SPICCR.bit.SPISWRESET = 1;         //Release SPI from reset
    SpiaRegs.SPIPRI.bit.FREE = 1;
    SpiaRegs.SPIPRI.bit.SOFT = 1;

    /* Set DI and CS high and apply more than 74 pulses to SCLK for the card */
    /* to be able to accept a native command. */
    //send_initial_clock_train();

    //DELAY_US(50);

    PowerFlag = 1;
}


// set the SPI speed to the max setting
static
void set_max_speed(void)			//EDIT
{
    /* Disable the SPI*/
    //DevCfgRegs.DC9.bit.SPI_C = 0;
	SpicRegs.SPICCR.bit.SPISWRESET = 0;			//Place in reset

    /* Configure the SPI C port */
    SpicRegs.SPICTL.bit.CLK_PHASE = 0;
    SpicRegs.SPICCR.bit.CLKPOLARITY = 1;
    SpicRegs.SPICTL.bit.MASTER_SLAVE = 1;		//Set Master Mode

    SpicRegs.SPIBRR.all = 1; // <- (No enforced limit)

    SpicRegs.SPICCR.bit.SPICHAR = 0x7;			//8 bit char length
    SpicRegs.SPICTL.bit.TALK = 1;

    /* Enable the SPI */
    SpicRegs.SPICCR.bit.SPISWRESET = 1;			//Release from reset
    //DevCfgRegs.DC9.bit.SPI_C = 1;
}

static
void power_off (void)
{
    PowerFlag = 0;
}

static
int chk_power(void)        /* Socket power state: 0=off, 1=on */
{
    return PowerFlag;
}



/*-----------------------------------------------------------------------*/
/* Receive a data packet from MMC                                        */
/*-----------------------------------------------------------------------*/

static
BOOL rcvr_datablock (
    BYTE *buff,            /* Data buffer to store received data */
    UINT btr            /* Byte count (must be even number) */
)
{
    BYTE token;


    Timer1 = 10;
    do                              /* Wait for data packet in timeout of 100ms */
    {
        token = rcvr_spi();
    }
    while ((token == 0xFF) && Timer1);
    if(token != 0xFE) return FALSE;    /* If not valid data token, retutn with error */

    do                              /* Receive the data block into buffer */
    {
        rcvr_spi_m(buff++);
        rcvr_spi_m(buff++);
    }while (btr -= 2);
    rcvr_spi();                        /* Discard CRC */
    rcvr_spi();

    return TRUE;                    /* Return with success */
}



/*-----------------------------------------------------------------------*/
/* Send a data packet to MMC                                             */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
static
BOOL xmit_datablock (
    const BYTE *buff,    /* 512 byte data block to be transmitted */
    BYTE token            /* Data/Stop token */
)
{
    BYTE resp, wc;


    if (wait_ready() != 0xFF) return FALSE;

    xmit_spi(token);                    /* Xmit data token */
    if (token != 0xFD)      /* Is data token */
    {
        wc = 0;
        do                              /* Xmit the 512 byte data block to MMC */
        {
            xmit_spi(*buff++);
            xmit_spi(*buff++);
        }
        while (--wc);
        xmit_spi(0xFF);                    /* CRC (Dummy) */
        xmit_spi(0xFF);
        resp = rcvr_spi();                /* Reveive data response */
        if ((resp & 0x1F) != 0x05)        /* If not accepted, return with error */
            return FALSE;
    }

    return TRUE;
}
#endif /* _READONLY */



static
BYTE send_cmd12 (void)
{
    BYTE n, res, val;

    /* For CMD12, we don't wait for the card to be idle before we send
     * the new command.
     */

    /* Send command packet - the argument for CMD12 is ignored. */
    xmit_spi(CMD12);
    xmit_spi(0);
    xmit_spi(0);
    xmit_spi(0);
    xmit_spi(0);
    xmit_spi(0);

    /* Read up to 10 bytes from the card, remembering the value read if it's
       not 0xFF */
    for(n = 0; n < 10; n++)
    {
        val = rcvr_spi();
        if(val != 0xFF)
        {
            res = val;
        }
    }

    return res;            /* Return with the response value */
}

/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
    BYTE drv        /* Physical drive nmuber (0) */
)
{
    BYTE n, ty, ocr[4];


    if (drv) return STA_NOINIT;            /* Supports only single drive */
    if (Stat & STA_NODISK) return Stat;    /* No card in the socket */

    power_on();                            /* Force socket power on */
    send_initial_clock_train();

    GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;
    ty = 0;
    if (send_cmd(CMD0, 0) == 1)              /* Enter Idle state */
    {
        Timer1 = 100;                        /* Initialization timeout of 1000 msec */
        if (send_cmd(CMD8, 0x1AA) == 1)      /* SDC Ver2+ */
        {
            for (n = 0; n < 4; n++) ocr[n] = rcvr_spi();
            if (ocr[2] == 0x01 && ocr[3] == 0xAA)      /* The card can work at vdd range of 2.7-3.6V */
            {
                do
                {
                    if (send_cmd(CMD55, 0) <= 1 && send_cmd(CMD41, 1UL << 30) == 0)    break;    /* ACMD41 with HCS bit */
                }
                while (Timer1);
                if (Timer1 && send_cmd(CMD58, 0) == 0)      /* Check CCS bit */
                {
                    for (n = 0; n < 4; n++) ocr[n] = rcvr_spi();
                    ty = (ocr[0] & 0x40) ? 6 : 2;
                }
            }
        }
        else                                /* SDC Ver1 or MMC */
        {
            ty = (send_cmd(CMD55, 0) <= 1 && send_cmd(CMD41, 0) <= 1) ? 2 : 1;    /* SDC : MMC */
            do
            {
                if (ty == 2)
                {
                    if (send_cmd(CMD55, 0) <= 1 && send_cmd(CMD41, 0) == 0) break;    /* ACMD41 */
                }
                else
                {
                    if (send_cmd(CMD1, 0) == 0) break;                                /* CMD1 */
                }
            }
            while (Timer1);
            if (!Timer1 || send_cmd(CMD16, 512) != 0)    /* Select R/W block length */
                ty = 0;
        }
    }
    CardType = ty;
    GpioDataRegs.GPASET.bit.GPIO19 = 1;            /* CS = H */
    rcvr_spi();            /* Idle (Release DO) */

    if (ty)              /* Initialization succeded */
    {
        Stat &= ~STA_NOINIT;        /* Clear STA_NOINIT */
        set_max_speed();
    }
    else                /* Initialization failed */
    {
        power_off();
    }

    return Stat;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
    BYTE drv        /* Physical drive number (0) */
)
{
    if (drv) return STA_NOINIT;        /* Supports only single drive */
    return Stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
    BYTE drv,            /* Physical drive nmuber (0) */
    BYTE *buff,            /* Pointer to the data buffer to store read data */
    DWORD sector,        /* Start sector number (LBA) */
    BYTE count            /* Sector count (1..255) */
)
{
    if (drv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    if (!(CardType & 4)) sector *= 512;    /* Convert to byte address if needed */

    GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;

    if (count == 1)      /* Single block read */
    {
        if ((send_cmd(CMD17, sector) == 0)    /* READ_SINGLE_BLOCK */
                && rcvr_datablock(buff, 512))
            count = 0;
    }
    else                  /* Multiple block read */
    {
        if (send_cmd(CMD18, sector) == 0)      /* READ_MULTIPLE_BLOCK */
        {
            do
            {
                if (!rcvr_datablock(buff, 512)) break;
                buff += 512;
            }
            while (--count);
            send_cmd12();                /* STOP_TRANSMISSION */
        }
    }

    GpioDataRegs.GPASET.bit.GPIO19 = 1;
    rcvr_spi();            /* Idle (Release DO) */

    return count ? RES_ERROR : RES_OK;
}


/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
DRESULT disk_write (
    BYTE drv,            /* Physical drive nmuber (0) */
    const BYTE *buff,    /* Pointer to the data to be written */
    DWORD sector,        /* Start sector number (LBA) */
    BYTE count            /* Sector count (1..255) */
)
{
    if (drv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;
    if (Stat & STA_PROTECT) return RES_WRPRT;

    if (!(CardType & 4)) sector *= 512;    /* Convert to byte address if needed */

    GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;

    if (count == 1)      /* Single block write */
    {
        if ((send_cmd(CMD24, sector) == 0)    /* WRITE_BLOCK */
                && xmit_datablock(buff, 0xFE))
            count = 0;
    }
    else                  /* Multiple block write */
    {
        if (CardType & 2)
        {
            send_cmd(CMD55, 0);
            send_cmd(CMD23, count);    /* ACMD23 */
        }
        if (send_cmd(CMD25, sector) == 0)      /* WRITE_MULTIPLE_BLOCK */
        {
            do
            {
                if (!xmit_datablock(buff, 0xFC)) break;
                buff += 512;
            }
            while (--count);
            if (!xmit_datablock(0, 0xFD))    /* STOP_TRAN token */
                count = 1;
        }
    }

    GpioDataRegs.GPASET.bit.GPIO19 = 1;
    rcvr_spi();            /* Idle (Release DO) */

    return count ? RES_ERROR : RES_OK;
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
    BYTE drv,        /* Physical drive nmuber (0) */
    BYTE ctrl,        /* Control code */
    void *buff        /* Buffer to send/receive control data */
)
{
    DRESULT res;
    BYTE n, csd[16], *ptr = buff;
    WORD csize;


    if (drv) return RES_PARERR;

    res = RES_ERROR;

    if (ctrl == CTRL_POWER)
    {
        switch (*ptr)
        {
        case 0:        /* Sub control code == 0 (POWER_OFF) */
            if (chk_power())
                power_off();        /* Power off */
            res = RES_OK;
            break;
        case 1:        /* Sub control code == 1 (POWER_ON) */
            power_on();                /* Power on */
            res = RES_OK;
            break;
        case 2:        /* Sub control code == 2 (POWER_GET) */
            *(ptr+1) = (BYTE)chk_power();
            res = RES_OK;
            break;
        default :
            res = RES_PARERR;
        }
    }
    else
    {
        if (Stat & STA_NOINIT) return RES_NOTRDY;

        GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;

        switch (ctrl)
        {
        case GET_SECTOR_COUNT :    /* Get number of sectors on the disk (DWORD) */
            if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16))
            {
                if ((csd[0] >> 6) == 1)      /* SDC ver 2.00 */
                {
                    csize = csd[9] + ((WORD)csd[8] << 8) + 1;
                    *(DWORD*)buff = (DWORD)csize << 10;
                }
                else                        /* MMC or SDC ver 1.XX */
                {
                    n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
                    csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
                    *(DWORD*)buff = (DWORD)csize << (n - 9);
                }
                res = RES_OK;
            }
            break;

        case GET_SECTOR_SIZE :    /* Get sectors on the disk (WORD) */
            *(WORD*)buff = 512;
            res = RES_OK;
            break;

        case CTRL_SYNC :    /* Make sure that data has been written */
            if (wait_ready() == 0xFF)
                res = RES_OK;
            break;

        case MMC_GET_CSD :    /* Receive CSD as a data block (16 bytes) */
            if (send_cmd(CMD9, 0) == 0        /* READ_CSD */
                    && rcvr_datablock(ptr, 16))
                res = RES_OK;
            break;

        case MMC_GET_CID :    /* Receive CID as a data block (16 bytes) */
            if (send_cmd(CMD10, 0) == 0        /* READ_CID */
                    && rcvr_datablock(ptr, 16))
                res = RES_OK;
            break;

        case MMC_GET_OCR :    /* Receive OCR as an R3 resp (4 bytes) */
            if (send_cmd(CMD58, 0) == 0)      /* READ_OCR */
            {
                for (n = 0; n < 4; n++)
                    *ptr++ = rcvr_spi();
                res = RES_OK;
            }

//        case MMC_GET_TYPE :    /* Get card type flags (1 byte) */
//            *ptr = CardType;
//            res = RES_OK;
//            break;

        default:
            res = RES_PARERR;
        }

        GpioDataRegs.GPASET.bit.GPIO19 = 1;
        rcvr_spi();            /* Idle (Release DO) */
    }

    return res;
}
/*-----------------------------------------------------------------------*/
/* Device Timer Interrupt Procedure  (Platform dependent)                */
/*-----------------------------------------------------------------------*/
/* This function must be called in period of 10ms                        */

void disk_timerproc (void)
{
//    BYTE n, s;
    BYTE n;


    n = Timer1;                        /* 100Hz decrement timer */
    if (n) Timer1 = --n;
    n = Timer2;
    if (n) Timer2 = --n;

}





