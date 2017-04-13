/* Enclosed is all of the functions included in the Base_MCU.c */

#include "F28x_Project.h"
#include "metaTait_HighLevel.h"
#include "metaTait_SDCard.h"
#include "metaTait_SCI.h"
#include "metaTait_ISR.h"

#define Integration_Test        1


int rpm;                //Will house our RPM value that the hall effect sensor data helps calculate
int target = 0;         //Will be the value that the RPM is checked against
Uint16 bright = 0;
/*
extern struct I2CMSG *CurrentMsgPtr;
extern struct I2CMSG I2cMsgOut1;
extern struct I2CMSG I2cMsgIn1;
extern struct I2CMSG I2cMsgOut2;
extern struct I2CMSG I2cMsgIn2;
extern struct I2CMSG I2cMsgOut3;
extern struct I2CMSG I2cMsgIn3;
extern int passs;
extern int fails;
*/


void receive_data(void)
{

    Uint16 brightness;
    Uint16 start_cmd = 0;
//    Uint16 Error = 0;
    Uint16 targetrpm[4];
    Uint16 target_rpm = 0;


    image_data_send();                                  //Send Image Data to the Posts.


    //SciaRegs.SCITXBUF.all= 0;
    scia_xmit(1, 0);
	while (SciaRegs.SCIFFRX.bit.RXFFST == 0);
	targetrpm[3] = scia_receive();

	target_rpm = target_rpm + ((int) targetrpm[3] * 1000);

	//SciaRegs.SCITXBUF.all= 5;
	while (SciaRegs.SCIFFRX.bit.RXFFST == 0);
	targetrpm[2] = scia_receive();
	target_rpm = target_rpm + ((int) targetrpm[2] * 100);

	//SciaRegs.SCITXBUF.all= 1;
	while (SciaRegs.SCIFFRX.bit.RXFFST == 0);
	targetrpm[1] = scia_receive();
	target_rpm = target_rpm + ((int) targetrpm[1] * 10);

	//SciaRegs.SCITXBUF.all= 0;
	while (SciaRegs.SCIFFRX.bit.RXFFST == 0);
	targetrpm[0] = scia_receive();
	target_rpm = target_rpm + ((int) targetrpm[0] * 1);
	target = target_rpm;                 //Put RPM value into target rpm global.

	scia_xmit(3, target);

	scia_xmit(2, 0);
	//SciaRegs.SCITXBUF.all= 0x1F;
	while (SciaRegs.SCIFFRX.bit.RXFFST == 0);
	brightness = scia_receive();
	if (brightness < 0 || brightness > 0x1F)
		for (;;);

	scia_xmit(0, 0);
	//SciaRegs.SCITXBUF.all= 1;
	while (SciaRegs.SCIFFRX.bit.RXFFST == 0);
	start_cmd = scia_receive();                //Receive Start Command via UART.
	if (start_cmd != 1)
		for (;;);

/*
    while(!passs && !fails)
    {

    if(I2cMsgOut1.MsgStatus == I2C_MSGSTAT_SEND_WITHSTOP)
    {
            Error = I2C_Write(&I2cMsgOut1);

            //
            // If communication is correctly initiated, set msg status to busy
            // and update CurrentMsgPtr for the __interrupt service routine.
            // Otherwise, do nothing and try again next loop. Once message is
            // initiated, the I2C __interrupts will handle the rest. Search for
            // ICINTR1A_ISR in the i2c_eeprom_isr.c file.
            //
            if (Error == I2C_SUCCESS)
            {
               CurrentMsgPtr = &I2cMsgOut1;
               I2cMsgOut1.MsgStatus = I2C_MSGSTAT_WRITE_BUSY;
            }
    }
    if (I2cMsgOut1.MsgStatus == I2C_MSGSTAT_INACTIVE)
          {
             //
             // Check incoming message status.
             //
             if(I2cMsgIn1.MsgStatus == I2C_MSGSTAT_SEND_NOSTOP)
             {
                //
                // EEPROM address setup portion
                //
                while(I2C_Read(&I2cMsgIn1) != I2C_SUCCESS)
                {
                   //
                   // Maybe setup an attempt counter to break an infinite while
                   // loop. The EEPROM will send back a NACK while it is performing
                   // a write operation. Even though the write communique is
                   // complete at this point, the EEPROM could still be busy
                   // programming the data. Therefore, multiple attempts are
                   // necessary.
                   //
                }
                //
                // Update current message pointer and message status
                //
                CurrentMsgPtr = &I2cMsgIn1;
                I2cMsgIn1.MsgStatus = I2C_MSGSTAT_SEND_NOSTOP_BUSY;
             }
             //
             // Once message has progressed past setting up the internal address
             // of the EEPROM, send a restart to read the data bytes from the
             // EEPROM. Complete the communique with a stop bit. MsgStatus is
             // updated in the __interrupt service routine.
             //
             else if(I2cMsgIn1.MsgStatus == I2C_MSGSTAT_RESTART)
             {
                //
                // Read data portion
                //
                while(I2C_Read(&I2cMsgIn1) != I2C_SUCCESS)
                {
                   //
                   // Maybe setup an attempt counter to break an infinite while
                   // loop.
                   //
                }
                //
                // Update current message pointer and message status
                //
                CurrentMsgPtr = &I2cMsgIn1;
                I2cMsgIn1.MsgStatus = I2C_MSGSTAT_READ_BUSY;
                Uint16 Input = I2caRegs.I2CDRR.all;
             }
          }
    }
    int k = 0;
    while(k!= 0);
    k = 1;
    */

}


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
    int m = 0;
    int size;
    Uint16 holder = 0;
    Uint16 max_receive_bytes = 2048;


#if Integration_Test
    Uint16 total_data_size = 32;
#endif

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

        while(k < size/2)                         //Put it into I2C format to send out.
        {
            holder = buff[l] << 8;
            SD_Data[k]  = holder || buff[l+1];
            l = l+2;
            k++;
        }
        while(m < size)
        {
/*
            I2cMsgOut1.NumOfBytes = 2;
            I2cMsgOut1.MsgBuffer[0] = SD_Data[m];

            I2cMsgOut2.NumOfBytes = 2;
            I2cMsgOut2.MsgBuffer[0] = SD_Data[m];

            I2cMsgOut3.NumOfBytes = 2;
            I2cMsgOut3.MsgBuffer[0] = SD_Data[m];

            I2C_Write(&I2cMsgOut1);
            I2C_Write(&I2cMsgOut2);
            I2C_Write(&I2cMsgOut3);
*/
            m++;
        }
        m = 0;
        i = i + size;
        j++;
        DELAY_US(20000000);         //Give the Post MCU's time to get the stuff into FRAM.
        DELAY_US(20000000);
    }
}
//##################### End of Send Image Data ############################################################################




