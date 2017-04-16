#include "F28x_Project.h"
#include "metaTait_SPI.h"

void spi_fifo_init()
{
    //
    // Initialize SPI FIFO registers
    //
    SpiaRegs.SPIFFTX.all = 0xC022;    // Enable FIFOs, set TX FIFO level to 4
    SpiaRegs.SPIFFRX.all = 0x0022;    // Set RX FIFO level to 4
    SpiaRegs.SPIFFCT.all = 0x00;

    SpiaRegs.SPIFFTX.bit.TXFIFO=1;
    SpiaRegs.SPIFFRX.bit.RXFIFORESET=1;

    SpibRegs.SPIFFTX.all = 0xC022;    // Enable FIFOs, set TX FIFO level to 4
    SpibRegs.SPIFFRX.all = 0x0022;    // Set RX FIFO level to 4
    SpibRegs.SPIFFCT.all = 0x00;

    SpibRegs.SPIFFTX.bit.TXFIFO=1;
    SpibRegs.SPIFFRX.bit.RXFIFORESET=1;

    SpicRegs.SPIFFTX.all = 0xC022;    // Enable FIFOs, set TX FIFO level to 4
    SpicRegs.SPIFFRX.all = 0x0022;    // Set RX FIFO level to 4
    SpicRegs.SPIFFCT.all = 0x00;

    SpicRegs.SPIFFTX.bit.TXFIFO=1;
    SpicRegs.SPIFFRX.bit.RXFIFORESET=1;
}



void InitSpi(void)
{
    // Initialize SPI-A

    // Set reset low before configuration changes
    // Clock polarity (0 == rising, 1 == falling)
    // 16-bit character
    // Enable loop-back
    SpiaRegs.SPICCR.bit.SPISWRESET = 0;
    SpiaRegs.SPICCR.bit.CLKPOLARITY = 0;
    SpiaRegs.SPICCR.bit.SPICHAR = (16-1);
    SpiaRegs.SPICCR.bit.SPILBK = 1;

    // Enable master (0 == slave, 1 == master)
    // Enable transmission (Talk)
    // Clock phase (0 == normal, 1 == delayed)
    // SPI interrupts are disabled
    SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1;
    SpiaRegs.SPICTL.bit.TALK = 1;
    SpiaRegs.SPICTL.bit.CLK_PHASE = 0;
    SpiaRegs.SPICTL.bit.SPIINTENA = 0;

    // Set the baud rate
    SpiaRegs.SPIBRR.bit.SPI_BIT_RATE = SPI_BRR;

    // Set FREE bit
    // Halting on a breakpoint will not halt the SPI
    SpiaRegs.SPIPRI.bit.FREE = 1;

    // Release the SPI from reset
    SpiaRegs.SPICCR.bit.SPISWRESET = 1;


    // Initialize SPI-B

    // Set reset low before configuration changes
    // Clock polarity (0 == rising, 1 == falling)
    // 16-bit character
    // Enable loop-back
    SpibRegs.SPICCR.bit.SPISWRESET = 0;
    SpibRegs.SPICCR.bit.CLKPOLARITY = 0;
    SpibRegs.SPICCR.bit.SPICHAR = (16-1);
    SpibRegs.SPICCR.bit.SPILBK = 1;

    // Enable master (0 == slave, 1 == master)
    // Enable transmission (Talk)
    // Clock phase (0 == normal, 1 == delayed)
    // SPI interrupts are disabled
    SpibRegs.SPICTL.bit.MASTER_SLAVE = 1;
    SpibRegs.SPICTL.bit.TALK = 1;
    SpibRegs.SPICTL.bit.CLK_PHASE = 0;
    SpibRegs.SPICTL.bit.SPIINTENA = 0;

    // Set the baud rate
    SpibRegs.SPIBRR.bit.SPI_BIT_RATE = SPI_BRR;

    // Set FREE bit
    // Halting on a breakpoint will not halt the SPI
    SpibRegs.SPIPRI.bit.FREE = 1;

    // Release the SPI from reset
    SpibRegs.SPICCR.bit.SPISWRESET = 1;


    // Initialize SPI-C

    // Set reset low before configuration changes
    // Clock polarity (0 == rising, 1 == falling)
    // 16-bit character
    // Enable loop-back
    SpicRegs.SPICCR.bit.SPISWRESET = 0;
    SpicRegs.SPICCR.bit.CLKPOLARITY = 0;
    SpicRegs.SPICCR.bit.SPICHAR = (16-1);
    SpicRegs.SPICCR.bit.SPILBK = 1;

    // Enable master (0 == slave, 1 == master)
    // Enable transmission (Talk)
    // Clock phase (0 == normal, 1 == delayed)
    // SPI interrupts are disabled
    SpicRegs.SPICTL.bit.MASTER_SLAVE = 1;
    SpicRegs.SPICTL.bit.TALK = 1;
    SpicRegs.SPICTL.bit.CLK_PHASE = 0;
    SpicRegs.SPICTL.bit.SPIINTENA = 0;

    // Set the baud rate
    SpicRegs.SPIBRR.bit.SPI_BIT_RATE = SPI_BRR;

    // Set FREE bit
    // Halting on a breakpoint will not halt the SPI
    SpicRegs.SPIPRI.bit.FREE = 1;

    // Release the SPI from reset
    SpicRegs.SPICCR.bit.SPISWRESET = 1;
}




void InitSpiGpio()
{
   EALLOW;

//For SPI-A
    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;  // Enable pull-up on GPIO16 (SPISIMOA)
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;  // Enable pull-up on GPIO17 (SPISOMIA)
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;  // Enable pull-up on GPIO18 (SPICLKA)
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;  // Enable pull-up on GPIO19 (SPISTEA)


    GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3;  // Enable pull-up on GPIO16 (SPISIMOA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3;  // Enable pull-up on GPIO17 (SPISOMIA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3;  // Enable pull-up on GPIO18 (SPICLKA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;  // Enable pull-up on GPIO19 (SPISTEA)


    GpioCtrlRegs.GPAGMUX2.bit.GPIO16 = 3;  // Enable pull-up on GPIO16 (SPISIMOA)
    GpioCtrlRegs.GPAGMUX2.bit.GPIO17 = 3;  // Enable pull-up on GPIO17 (SPISOMIA)
    GpioCtrlRegs.GPAGMUX2.bit.GPIO18 = 3;  // Enable pull-up on GPIO18 (SPICLKA)
    GpioCtrlRegs.GPAGMUX2.bit.GPIO19 = 3;  // Enable pull-up on GPIO19 (SPISTEA)


    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 3;  // Enable pull-up on GPIO16 (SPISIMOA)
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 3;  // Enable pull-up on GPIO16 (SPISOMIA)
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 3;  // Enable pull-up on GPIO18 (SPICLKA)
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 3;  // Enable pull-up on GPIO16 (SPISTEA)




//For SPI-B and SPI-C
    GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;  // Enable pull-up on GPIO16 (SPISIMOB)
    GpioCtrlRegs.GPCPUD.bit.GPIO65 = 0;  // Enable pull-up on GPIO18 (SPICLKB)
    GpioCtrlRegs.GPCPUD.bit.GPIO69 = 0;  // Enable pull-up on GPIO16 (SPISIMOC)
    GpioCtrlRegs.GPCPUD.bit.GPIO71 = 0;  // Enable pull-up on GPIO18 (SPICLKC)

    GpioCtrlRegs.GPBQSEL2.bit.GPIO63 = 3;  // Enable pull-up on GPIO16 (SPISIMOB)
    GpioCtrlRegs.GPCQSEL1.bit.GPIO65 = 3;  // Enable pull-up on GPIO18 (SPICLKB)
    GpioCtrlRegs.GPCQSEL1.bit.GPIO69 = 3;  // Enable pull-up on GPIO16 (SPISIMOC)
    GpioCtrlRegs.GPCQSEL1.bit.GPIO71 = 3;  // Enable pull-up on GPIO18 (SPICLKC)

    GpioCtrlRegs.GPBGMUX2.bit.GPIO63 = 3;  // Enable pull-up on GPIO16 (SPISIMOB)
    GpioCtrlRegs.GPCGMUX1.bit.GPIO65 = 3;  // Enable pull-up on GPIO18 (SPICLKB)
    GpioCtrlRegs.GPCGMUX1.bit.GPIO69 = 3;  // Enable pull-up on GPIO16 (SPISIMOC)
    GpioCtrlRegs.GPCGMUX1.bit.GPIO71 = 3;  // Enable pull-up on GPIO18 (SPICLKC)

    GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 3;  // Enable pull-up on GPIO16 (SPISIMOB)
    GpioCtrlRegs.GPCMUX1.bit.GPIO65 = 3;  // Enable pull-up on GPIO18 (SPICLKB)
    GpioCtrlRegs.GPCMUX1.bit.GPIO69 = 3;  // Enable pull-up on GPIO16 (SPISIMOC)
    GpioCtrlRegs.GPCMUX1.bit.GPIO71 = 3;  // Enable pull-up on GPIO18 (SPICLKC)

    EDIS;
}




void spib_xmit(Uint16 data)
{
	 SpibRegs.SPITXBUF = data;
}

void spic_xmit(Uint16 data)
{
	 SpicRegs.SPITXBUF = data;
}

