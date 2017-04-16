#include "metaTait_SCI.h"
#include "F28x_Project.h"


Uint16 scia_receive()                                       //UART Receive
{
    Uint16 b = ScibRegs.SCIRXBUF.all;
    return b;
}


void sci_init(void)
{

    GPIO_SetupPinMux(11, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(11, GPIO_INPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(10, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(10, GPIO_OUTPUT, GPIO_ASYNC);
    ScibRegs.SCICCR.all = 0x0007;  // 1 stop bit,  No loopback, No parity, 8 char bits, async mode, idle-line protocol
    ScibRegs.SCICTL1.all = 0x0003; // enable TX, RX, internal SCICLK   Disable RX ERR, SLEEP, TXWAKE
    ScibRegs.SCICTL2.all = 0x0003;
    ScibRegs.SCICTL2.bit.TXINTENA = 1;
    ScibRegs.SCICTL2.bit.RXBKINTENA = 1;
    ScibRegs.SCIHBAUD.all = 0x0002;             //0x0002 Determining Baud Rate These Correspond to 200 MHz
    ScibRegs.SCILBAUD.all = 0x008B;           //0x008B
    ScibRegs.SCICTL1.all = 0x0023;     // Relinquish SCI from Reset
    ScibRegs.SCIFFTX.all = 0xE040;      //SCIA FIFO Init
    ScibRegs.SCIFFRX.all = 0x2044;
    ScibRegs.SCIFFCT.all = 0x0;
}


void scia_xmit(int command, int var)
{
    int i = 0;
    switch(command)
    {
        case 0:
        {
            char comm[] = {"Ready to begin? Enter 1 to Start. \r\n"};
            for(i= 0; i < sizeof(comm); i++)
            {
                ScibRegs.SCITXBUF.all= comm[i];
            }
            break;
        }

        case 1:
        {
            char comm[] = {"Enter the RPM value, one character at a time \r\n"};
            for(i= 0; i < sizeof(comm); i++)
            {
                ScibRegs.SCITXBUF.all= comm[i];
            }
            break;
        }

        case 2:
        {
            char comm[] = {"Enter the global brightness value \r\n"};
            for(i= 0; i < sizeof(comm); i++)
            {
                ScibRegs.SCITXBUF.all= comm[i];
            }
            break;
        }

        case 3:
        {
            char comm[] = {"Target rpm value:"};
            for(i= 0; i < sizeof(comm); i++)
            {
                ScibRegs.SCITXBUF.all= comm[i];
            }
            char com = (char)var;
            ScibRegs.SCITXBUF.all= com;
            break;
        }

        case 4:
        {
            while (ScibRegs.SCIFFTX.bit.TXFFST != 0) {}
            char com = (char)var;
            ScibRegs.SCITXBUF.all= com;
            break;
        }
    }
}



