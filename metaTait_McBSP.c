#include "F28x_Project.h"
#include "metaTait_McBSP.h"

void init_mcbsp_spi()
{
    //
    // McBSP-A register settings
    //
    McbspaRegs.SPCR2.all = 0x0000;       // Reset FS generator, sample rate
                                         // generator & transmitter
    McbspaRegs.SPCR1.all = 0x0000;       // Reset Receiver, Right justify word,
                                         // Digital loopback dis.
    McbspaRegs.PCR.all = 0x0F08;         //(CLKXM=CLKRM=FSXM=FSRM= 1, FSXP = 1)
    McbspaRegs.SPCR1.bit.DLB = 1;
    McbspaRegs.SPCR1.bit.CLKSTP = 2;     // Together with CLKXP/CLKRP
                                         // determines clocking scheme
    McbspaRegs.PCR.bit.CLKXP = 0;        // CPOL = 0, CPHA = 0 rising edge
                                         // no delay
    McbspaRegs.PCR.bit.CLKRP = 0;
    McbspaRegs.RCR2.bit.RDATDLY = 01;    // FSX setup time 1 in master mode.
                                         // 0 for slave mode (Receive)
    McbspaRegs.XCR2.bit.XDATDLY = 01;    // FSX setup time 1 in master mode.
                                         // 0 for slave mode (Transmit)

    McbspaRegs.RCR1.bit.RWDLEN1 = 2;     // 16-bit read
    McbspaRegs.XCR1.bit.XWDLEN1 = 2;     // 16-bit write

    McbspaRegs.SRGR2.all = 0x2000;       // CLKSM=1, FPER = 1 CLKG periods
    McbspaRegs.SRGR1.all = 0x000F;       // Frame Width = 1 CLKG period,
                                         // CLKGDV=16

    McbspaRegs.SPCR2.bit.GRST = 1;       // Enable the sample rate generator
    delay_loop();                        // Wait at least 2 SRG clock cycles
    McbspaRegs.SPCR2.bit.XRST = 1;       // Release TX from Reset
    McbspaRegs.SPCR1.bit.RRST = 1;       // Release RX from Reset
    McbspaRegs.SPCR2.bit.FRST = 1;       // Frame Sync Generator reset
}




void InitMcbspaGpio(void)
{
#ifdef CPU1
    EALLOW;

    //
    // This specifies which of the possible GPIO pins will be
    // McBSPA functional pins. Comment out unwanted connections.
    // Set qualification for selected input pins to asynchronous only
    // This will select asynchronous (no qualification) for the selected pins.
    //

    //
    // MDXA
    // GPIO20
    // GPIO84
    //
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 2;
    //GpioCtrlRegs.GPCGMUX2.bit.GPIO84 = 3;
    //GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 3;

    //
    // MDRA
    // GPIO21 with asynchronous qualification
    // GPIO85 with asynchronous qualification
    //
    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 2;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 3;
    //GpioCtrlRegs.GPCGMUX2.bit.GPIO85 = 3;
    //GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 3;
    //GpioCtrlRegs.GPCQSEL2.bit.GPIO85 = 3;

    //
    // MCLKXA
    // GPIO22 with asynchronous qualification
    // GPIO86 with asynchronous qualification
    //
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 2;
    //GpioCtrlRegs.GPAQSEL2.bit.GPIO22 = 3;
    //GpioCtrlRegs.GPCGMUX2.bit.GPIO86 = 3;
    //GpioCtrlRegs.GPCMUX2.bit.GPIO86 = 3;
    //GpioCtrlRegs.GPCQSEL2.bit.GPIO86 = 3;

    //
    // MCLKRA
    // Select one of the following
    // GPIO7 with asynchronous qualification
    // GPIO58 with asynchronous qualification
    //
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 2;
    GpioCtrlRegs.GPAQSEL1.bit.GPIO7 = 3;
    //GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 1;
    //GpioCtrlRegs.GPBQSEL2.bit.GPIO58 = 3;

    //
    // MFSXA
    // GPIO23 with asynchronous qualification
    // GPIO87 with asynchronous qualification
    //
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 2;
    //GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;
    //GpioCtrlRegs.GPCGMUX2.bit.GPIO87 = 3;
    //GpioCtrlRegs.GPCMUX2.bit.GPIO87 = 3;
    //GpioCtrlRegs.GPCQSEL2.bit.GPIO87 = 3;

    //
    // MFSRA
    // Select one of the following
    // GPIO5 with asynchronous qualification
    // GPIO59 with asynchronous qualification
    //
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 2;
    GpioCtrlRegs.GPAQSEL1.bit.GPIO5 = 3;
    //GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 1;
    //GpioCtrlRegs.GPBQSEL2.bit.GPIO59 = 3;

    EDIS;
#endif
}



void delay_loop(void)
{
    long i;
    for (i = 0; i < MCBSP_INIT_DELAY; i++) {}
}

//
// clkg_delay_loop - Delay function (at least 2 CLKG cycles)
//                   Required in McBSP init
//
void clkg_delay_loop(void)
{
    long i;
    for (i = 0; i < MCBSP_CLKG_DELAY; i++) {}
}


void mcbsp_xmit(int a) /*int b);*/
{
    //McbspaRegs.DXR2.all = b;
    McbspaRegs.DXR1.all = a;
}


