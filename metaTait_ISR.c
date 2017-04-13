
#include "metaTait_ISR.h"
#include "F28x_Project.h"
#include "metaTait_SCI.h"

unsigned long counter1 = 0;
extern int rpm;
extern int target;



__interrupt void cpu_timer0_isr(void)
{
   counter1++;
   CpuTimer0.InterruptCount++;
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}



__interrupt void xint1_isr(void)
{

	float uts = 20000.0;											//This is the conversion from counter ticks to seconds.
	float seconds = counter1/uts; //time in seconds per rotation
	float f_refresh_window = seconds*100000000.0;
	unsigned long refresh_window = (int)f_refresh_window;
	float rotations_per_second = 1/seconds;
	float stm = 60.0;
	float arpm = rotations_per_second * stm; //rotations per minute.
	if(arpm < 0.0 || arpm > ((float)target)*1.2);
	else
	{
		rpm = (int)arpm + 1;
		if(rpm >= target)
		{
			Uint16 upper = (refresh_window & 0xFFFF0000) >> 16;
			Uint16 lower = (refresh_window & 0x0000FFFF);
			/*
			spia_xmit(upper);
			spia_xmit(lower);
			spib_xmit(upper);
			spib_xmit(lower);
			spic_xmit(upper);
			spic_xmit(lower);
			*/
		}
	}
	counter1 = 0;

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

void enable_pie_block(void)
{
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;
    PieCtrlRegs.PIEIER3.bit.INTx2 = 1;
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    PieCtrlRegs.PIEIER1.bit.INTx5 = 1;          // Enable PIE Group 1 INT5
}



void vector_table_init(void)
{
    EALLOW; // This is needed to write to EALLOW protected registers
//    PieVectTable.EPWM2_INT = &epwm2_isr;
    PieVectTable.TIMER0_INT = &cpu_timer0_isr;
    PieVectTable.XINT1_INT = &xint1_isr;
    EDIS;   // This is needed to disable write to EALLOW protected registers
}



void external_interrupt_init(void)
{
    //GPIO4 is Pin 93 on PCB
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;         // GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO4 = 0;          // input
    GpioCtrlRegs.GPAQSEL1.bit.GPIO4 = 0;        // XINT1 Synch to SYSCLKOUT only
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;

    GPIO_SetupXINT1Gpio(4);                     //Pin 19 is external interrupt
    XintRegs.XINT1CR.bit.POLARITY = 0;          // Falling edge interrupt
    XintRegs.XINT1CR.bit.ENABLE = 1;            // Enable XINT1
}








