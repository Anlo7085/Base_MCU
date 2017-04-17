/* Enclosed is all of the functions included in the Base_MCU.c */

#include "F28x_Project.h"
#include "metaTait_HighLevel.h"
#include "metaTait_SDCard.h"
#include "metaTait_SCI.h"
#include "metaTait_ISR.h"
#include "metaTait_SPI.h"
#include "metaTait_McBSP.h"


extern Uint16 led_brightness;


//##################### UART User Inputs ##################################################################################
void transmit(void)
{
    Uint16 brightness;
    //Uint16 start_cmd = 0;
    //Uint16 targetrpm[4];
    //Uint16 target_rpm = 0;


  //  image_data_send();                                  //Send Image Data to the Posts.

/*
    //SciaRegs.SCITXBUF.all= 0;
    scia_xmit(1, 0);
	while (ScibRegs.SCIFFRX.bit.RXFFST == 0);
	targetrpm[3] = scia_receive();

	target_rpm = target_rpm + ((int) targetrpm[3] * 1000);

	//SciaRegs.SCITXBUF.all= 5;
	while (ScibRegs.SCIFFRX.bit.RXFFST == 0);
	targetrpm[2] = scia_receive();
	target_rpm = target_rpm + ((int) targetrpm[2] * 100);

	//SciaRegs.SCITXBUF.all= 1;
	while (ScibRegs.SCIFFRX.bit.RXFFST == 0);
	targetrpm[1] = scia_receive();
	target_rpm = target_rpm + ((int) targetrpm[1] * 10);

	//SciaRegs.SCITXBUF.all= 0;
	while (ScibRegs.SCIFFRX.bit.RXFFST == 0);
	targetrpm[0] = scia_receive();
	target_rpm = target_rpm + ((int) targetrpm[0] * 1);
	target = target_rpm;                 //Put RPM value into target rpm global.

	scia_xmit(3, target);

	scia_xmit(2, 0);
	//SciaRegs.SCITXBUF.all= 0x1F;
	while (ScibRegs.SCIFFRX.bit.RXFFST == 0);
	brightness = scia_receive();

	scia_xmit(0, 0);
	//SciaRegs.SCITXBUF.all= 1;
	while (ScibRegs.SCIFFRX.bit.RXFFST == 0);
	while(start_cmd != 1)
		start_cmd = scia_receive();                //Receive Start Command via UART.
*/
    brightness = led_brightness;
	mcbsp_xmit(brightness);
	spib_xmit(brightness);
	spic_xmit(brightness);


}

//##################### End of UART User Inputs ##################################################################################


//##################### Send Image Data ##################################################################################

void image_data_send(void)
{

    BYTE* buff;
    Uint16 SD_Data[1024] = {0};
    UINT btr = 2048;
    unsigned long i = 0;
    int j= 0;
    int k = 0;
    int l = 0;
    int size;
    Uint16 holder = 0;
    Uint16 max_receive_bytes = 2048;
    Uint16 total_data_size = 32;


    power_on();
    send_initial_clock_train();
    wait_ready();

    while(j <= total_data_size/max_receive_bytes)
    {
        if(j != total_data_size/max_receive_bytes)
        {
            btr = max_receive_bytes;
            size = max_receive_bytes;
        }
        else
        {
            btr = total_data_size%max_receive_bytes;
            size= total_data_size%max_receive_bytes;
        }

        send_cmd(CMD17, i);
        BYTE token;
        Timer1 = 10;
        do                              // Wait for data packet in timeout of 100ms
        {
            token = rcvr_spi();
        }
        while ((token == 0xFF) && Timer1);
        if(token != 0xFE) return;    // If not valid data token, return with error

        do                              // Receive the data block into buffer
        {
            rcvr_spi_m(buff++); //buff++;
            rcvr_spi_m(buff++); //buff++;
        }
        while (btr -= 2);
        rcvr_spi();                        /// Discard CRC
        rcvr_spi();

        while(k < size/2)                         //Put it into SPI format to send out.
        {
            holder = buff[l] << 8;
            SD_Data[k]  = holder || buff[l+1];
            l = l+2;
            k++;
        }
        k = 0;
        while(k < size/2)
        {
        	mcbsp_xmit(SD_Data[k]);
        	spib_xmit(SD_Data[k]);
        	spic_xmit(SD_Data[k]);
            k++;
        }
        i = i + size;
        j++;
        DELAY_US(20000000);         //Give the Post MCU's time to get the stuff into FRAM.
    }
}
//##################### End of Send Image Data ############################################################################




