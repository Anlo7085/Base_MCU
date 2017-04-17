/*
 * metaTait_SPI.h
 *
 *  Created on: Apr 13, 2017
 *      Author: Drew
 */

#ifndef METATAIT_SPI_H_
#define METATAIT_SPI_H_

#define CPU_FRQ_120MHZ 0
#define CPU_FRQ_200MHZ 1

#if CPU_FRQ_200MHZ
#define SPI_BRR        16
#endif

#if CPU_FRQ_150MHZ
#define SPI_BRR        ((150E6 / 4) / 500E3) - 1
#endif

#if CPU_FRQ_120MHZ
#define SPI_BRR        ((120E6 / 4) / 500E3) - 1
#endif



void InitSpi(void);
void spi_fifo_init(void);
void InitSpiGpio(void);
void spib_xmit(Uint16);
void spic_xmit(Uint16);


#endif /* METATAIT_SPI_H_ */
