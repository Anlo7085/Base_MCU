/*
 * metaTait_McBSP.h
 *
 *  Created on: Apr 13, 2017
 *      Author: Drew
 */

#ifndef METATAIT_MCBSP_H_
#define METATAIT_MCBSP_H_


#define CPU_SPD              190E6
#define MCBSP_SRG_FREQ       CPU_SPD/4   // SRG input is LSPCLK (SYSCLKOUT/4)
                                         // for examples
#define CLKGDV_VAL           1

// # of CPU cycles in 2 SRG cycles-init delay
#define MCBSP_INIT_DELAY     2*(CPU_SPD/MCBSP_SRG_FREQ)

// # of CPU cycles in 2 CLKG cycles-init delay
#define MCBSP_CLKG_DELAY     2*(CPU_SPD/(MCBSP_SRG_FREQ/(1+CLKGDV_VAL)))




void InitMcbspbGpio(void);
void init_mcbsp_spi(void);
void mcbsp_xmit(int);

void delay_loop(void);
void clkg_delay_loop(void);



#endif /* METATAIT_MCBSP_H_ */
