#include "metaTait_ISR.h"
#include "F28x_Project.h"
#include "metaTait_SCI.h"
#include "metaTait_SPI.h"
#include "metaTait_McBSP.h"
#include "metaTait_SDCard.h"

#define round(x) (x<0.0)?(int16)(x-0.5):(int16)(x+0.5)

extern int timer_function;
unsigned long counter1 = 0;
float rpmS = -1;                //Will house our RPM value that the hall effect sensor data helps calculate
extern int target_rpm;         //Will be the value that the RPM is checked against
int numBad = 0;



__interrupt void cpu_timer0_isr(void)
{
	/*
   if(!timer_function)
   {
	   disk_timerproc();
   }
   else
   {*/
	   counter1++;
   //}
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}



__interrupt void xint2_isr(void)
{

	float uts = 20000.0;                                            //This is the conversion from counter ticks to seconds.
	float seconds = counter1/uts; //time in seconds per rotation
	float rotations_per_second = 1/seconds;
	float stm = 60.0;
	float rpmf = rotations_per_second * stm; //rotations per minute calc, including noise.
	if (rpmS == -1 && rpmf > 30 && rpmf < 35) {   //initial set of stable RPM
	    rpmS = rpmf;
	    numBad++;
        if(numBad > 10) {  //re-calibration if tracking along a noisy value for 10 consecutive updates
            rpmS = rpmf;
            numBad = 0;
        }
	}
	if(rpmf < rpmS*.8 || rpmf > rpmS*1.2);   //throw out rpm calcs greater than 20% off previous value
	else
	{
	    numBad = 0;
	    rpmS = rpmf;  //set most recent read to stable RPM decimal to be used
	    rotations_per_second = rpmf/60.0;
	    float refWin = (1.0/rotations_per_second)*1000000.0/216.0;

	    // NEW 16-BIT TRANSMIT REFRESH WINDOW:
	    Uint16 refresh_windo = (Uint16)(refWin); //cast to 16-bits for transmission
	    /*
	    if(refresh_windo >> 15)
	    {
	    	refresh_windo = refresh_windo | 0x0001;
	    }
	    else
	    {
	    	refresh_windo = refresh_windo & 0xFFFE;
	    }*/



	    while(McbspbRegs.SPCR2.bit.XRDY == 0);
	    mcbsp_xmit(refresh_windo);
	}

	counter1 = 0; //BASE
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








