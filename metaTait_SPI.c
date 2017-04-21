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

	//
	/*
	 GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;  // Enable pull-up on GPIO16 (SPISIMOA)
	 GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;  // Enable pull-up on GPIO18 (SPICLKA)
	 GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;
	 GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;
	 */
	// High speed SPI
	GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;  // Enable pull-up on GPIO16 (SPISIMOA)
	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;  // Enable pull-up on GPIO18 (SPICLKA)
	GpioCtrlRegs.GPBPUD.bit.GPIO58 = 0; // Enable pull-up on GPIO58 (SPICLKB)  		(GPIO58 = Pin15 on Dev Board)
	GpioCtrlRegs.GPBPUD.bit.GPIO59 = 0;	// Enable pull-up on GPIO66 (SPISTEB)  		(GPIO59 = Pin14 on Dev Board)
	//GpioCtrlRegs.GPBPUD.bit.GPIO60 = 0; // Enable pull-up on GPIO60 (SPISIMOB)  	(GPIO60 = Pin07 on Dev Board)
	GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;  // Enable pull-up on GPIO63 (SPISIMOB)  	(GPIO63 = Pin55 on Dev Board)
	//GpioCtrlRegs.GPCPUD.bit.GPIO65 = 0;   // Enable pull-up on GPIO65 (SPICLKB)  	(GPIO65 = Pin47 on Dev Board)
	GpioCtrlRegs.GPCPUD.bit.GPIO69 = 0; // Enable pull-up on GPIO69 (SPISIMOC)  	(GPIO69 = Pin49 on Dev Board)
	GpioCtrlRegs.GPCPUD.bit.GPIO71 = 0; // Enable pull-up on GPIO71 (SPICLKC)   	(GPIO71 = Pin02 on Dev Board)
	GpioCtrlRegs.GPCPUD.bit.GPIO72 = 0; // Enable pull-up on GPIO72 (SPISTEC)   	(GPIO72 = Pin13 on Dev Board)

	//
	// Set qualification for selected pins to asynch only
	//
	// This will select asynch (no qualification) for the selected pins.
	// Comment out other unwanted lines.
	//
	/*
	 // Regular SPI
	 GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3;  //Asynch input GPIO16 (SPISIMOA)
	 GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3;  //Asynch input GPIO18 (SPICLKA)
	 GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3;
	 GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;
	 */
	//  High Speed SPI
	GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3;  		// Asynch input GPIO16 (SPISIMOA)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3;  		// Asynch input GPIO18 (SPICLKA)
	GpioCtrlRegs.GPBQSEL2.bit.GPIO58 = 3; 		// Asynch input GPIO58 (SPICLKB)
	GpioCtrlRegs.GPBQSEL2.bit.GPIO59 = 3;		// Asynch input GPIO66 (SPISTEB)
	//GpioCtrlRegs.GPBQSEL2.bit.GPIO60 = 3;  	  // Asynch input GPIO60 (SPISIMOB)
	GpioCtrlRegs.GPBQSEL2.bit.GPIO63 = 3; 		// Asynch input GPIO63 (SPISIMOB)
	//GpioCtrlRegs.GPCQSEL1.bit.GPIO65 = 3;  	  // Asynch input GPIO65 (SPICLKB)
	GpioCtrlRegs.GPCQSEL1.bit.GPIO69 = 3; 		// Asynch input GPIO69 (SPISIMOC)
	GpioCtrlRegs.GPCQSEL1.bit.GPIO71 = 3; 		// Asynch input GPIO71 (SPICLKC)
	GpioCtrlRegs.GPCQSEL1.bit.GPIO72 = 3; 		// Asynch input GPIO71 (SPISTEC)

	//
	//Configure High Speed SPI-A, B and C pins using GPIO regs
	//
	// This specifies which of the possible GPIO pins will be SPI functional
	// pins.
	//
	/*
	 // Regular SPI
	 GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1;  // Configure GPIO16 as SPISIMOA
	 GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1;  // Configure GPIO18 as SPICLKA
	 GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1;
	 GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1;
	 */
	// High Speed SPI
	GpioCtrlRegs.GPAGMUX2.bit.GPIO16 = 0;  // Configure GPIO16 as SPISIMOA
	GpioCtrlRegs.GPAGMUX2.bit.GPIO18 = 0;  // Configure GPIO18 as SPICLKA
	GpioCtrlRegs.GPBGMUX2.bit.GPIO58 = 1; // Configure GPIO58 as SPICLKB
	GpioCtrlRegs.GPBGMUX2.bit.GPIO59 = 1; // Configure GPIO59 as SPISTEB
	//GpioCtrlRegs.GPBGMUX2.bit.GPIO60 = 1; // Configure GPIO60 as SPISIMOB
	GpioCtrlRegs.GPBGMUX2.bit.GPIO63 = 3; // Configure GPIO63 as SPISIMOB
	// GpioCtrlRegs.GPCGMUX1.bit.GPIO65 = 3; // Configure GPIO65 as SPICLKB
	GpioCtrlRegs.GPCGMUX1.bit.GPIO69 = 3; // Configure GPIO69 as SPISOMOC
	GpioCtrlRegs.GPCGMUX1.bit.GPIO71 = 3; // Configure GPIO71 as SPICLKC
	GpioCtrlRegs.GPCGMUX1.bit.GPIO72 = 3; // Configure GPIO71 as SPICLKC

	GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1;  // Configure GPIO16 as SPISIMOA
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1;  // Configure GPIO18 as SPICLKA
	GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 2; // Configure GPIO58 as SPICLKB
	GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 2; // Configure GPIO66 as SPISTEB
	//GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 2; // Configure GPIO60 as SPISIMOB
	GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 3; // Configure GPIO63 as SPISIMOB
	// GpioCtrlRegs.GPCMUX1.bit.GPIO65 = 3; // Configure GPIO65 as SPICLKB
	GpioCtrlRegs.GPCMUX1.bit.GPIO69 = 3; // Configure GPIO69 as SPISOMOC
	GpioCtrlRegs.GPCMUX1.bit.GPIO71 = 3; // Configure GPIO71 as SPICLKC
	GpioCtrlRegs.GPCMUX1.bit.GPIO72 = 3; // Configure GPIO71 as SPICLKC

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

