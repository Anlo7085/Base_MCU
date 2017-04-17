#include "metaTait_ISR.h"
#include "F28x_Project.h"
#include "metaTait_SCI.h"
#include "metaTait_SPI.h"
#include "metaTait_McBSP.h"

unsigned long counter1 = 0;
int rpm;                //Will house our RPM value that the hall effect sensor data helps calculate
extern int target_rpm;         //Will be the value that the RPM is checked against
int refresh_counter = 0;



__interrupt void cpu_timer0_isr(void)
{
   counter1++;
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}



__interrupt void xint2_isr(void)
{
	float uts = 20000.0;											//This is the conversion from counter ticks to seconds.
	float seconds = counter1/uts; //time in seconds per rotation
	float f_refresh_window = seconds*100000000.0;
	unsigned long refresh_window = (int)f_refresh_window;
	float rotations_per_second = 1/seconds;
	float stm = 60.0;
	float arpm = rotations_per_second * stm; //rotations per minute.
	if(arpm < 0.0 || arpm > ((float)target_rpm)*1.2);
	else
	{
		rpm = (int)arpm + 1;
		if(refresh_counter == 5)
		{
			refresh_counter == 0;
			if(rpm >= target_rpm)
			{
				Uint16 upper = (refresh_window & 0xFFFF0000) >> 16;
				Uint16 lower = (refresh_window & 0x0000FFFF);
				mcbsp_xmit(upper);
				mcbsp_xmit(lower);
				spib_xmit(upper);
				spib_xmit(lower);
				spic_xmit(upper);
				spic_xmit(lower);
			}
		}
		else
			refresh_counter++;
	}
	counter1 = 0;

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

void enable_pie_block(void)
{
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
    PieCtrlRegs.PIEIER3.bit.INTx2 = 1;
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    PieCtrlRegs.PIEIER1.bit.INTx5 = 1;          // Enable PIE Group 1 INT5
}



void vector_table_init(void)
{
    EALLOW; // This is needed to write to EALLOW protected registers
    PieVectTable.TIMER0_INT = &cpu_timer0_isr;
    PieVectTable.XINT2_INT = &xint2_isr;
    EDIS;   // This is needed to disable write to EALLOW protected registers
}



void external_interrupt_init(void)
{
    EALLOW;
    GpioCtrlRegs.GPCMUX1.bit.GPIO72 = 0;         // GPIO72
    GpioCtrlRegs.GPCDIR.bit.GPIO72 = 0;          // set as an input
    GpioCtrlRegs.GPCQSEL1.bit.GPIO72 = 0;        // XINT1 Synch to SYSCLKOUT only
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;

	GPIO_SetupXINT2Gpio(72);					//GPIO72 chosen as interrupt pin
    XintRegs.XINT2CR.bit.POLARITY = 0;          // Falling edge interrupt
	XintRegs.XINT2CR.bit.ENABLE = 1;            // Enable XINT1
}








