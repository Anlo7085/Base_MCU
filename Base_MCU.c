#include "F28x_Project.h"
#include "metaTait_HighLevel.h"          //Houses all of the prototypes, structures, and #defines
#include "metaTait_ISR.h"
#include "metaTait_SCI.h"
#include "metaTait_SPI.h"
#include "metaTait_McBSP.h"


//#####################  User Changeable Values Here ###########################################################################


Uint16 led_brightness_scalar = 0;	//This determines the brightness of the LED's, sent to the post before runtime. Max value of 0 for brightness
Uint16 target_rpm = 500;		//This is important for the extern interrupt calculations (located in metaTait_ISR.c)


//##############################################################################################################################
//extern int timer_function;

int main(void)
{

	InitSysCtrl();		//This is where the PLL Clock is initialized
	InitCpuTimers();   	//Enable CPU Timers
	InitGpio();			//Enable GPIO Pins

	spi_fifo_init();	//Enable SPI FIFO Buffers
	InitSpi();			//Initialize SPI
	InitSpiGpio();		//Initialize our chosen pins to be SPI pins

	InitMcbspbGpio();	//Enable chosen pins to be McBSP.
	init_mcbsp_spi();	//Enable McBSP to be SPI

	DINT;
	InitPieCtrl();

	// Disable CPU __interrupts and clear all CPU __interrupt flags:
	IER = 0x0000;
	IFR = 0x0000;

	InitPieVectTable();					//Initialize PIE vector table

	//sci_init();                        //Initialize UART

	EINT;
	// Enable Global interrupt INTM
	ERTM;
	// Enable Global realtime interrupt DBGM
	int o = 0;
	for(o = 0; o < 1; o++)
		DELAY_US(200000);

	//image_data_send();
	transmit();     //Transmit 5-bit User Set Brightness Value to Post MCU's

	DINT;			//Disable Interrupts

	ConfigCpuTimer(&CpuTimer0, 190, 50); 	//Initialize CPU Timer Interrupt to 50 us, 190 MHz Clock Frequency
	CpuTimer0Regs.TCR.all = 0x4001;

	vector_table_init();                     //Enable vector table of interrupts

	EALLOW;
	CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
	EDIS;

	external_interrupt_init();				//Enable Registers for External GPIO Hall Effect Sensor Interrupt

	IER |= M_INT3;
	IER |= M_INT1;

	enable_pie_block();						//Enable PIE for interrupts

	EINT;
	// Enable Global interrupt INTM
	ERTM;
	// Enable Global realtime interrupt DBGM

	//timer_function = 1;
	for (;;){}								//Stay here forever.
}
