/*
 * Mp3.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: lenovo 1
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#undef F_CPU
#define F_CPU 1000000
#include <util/delay.h>
#include "Std_types.h"
#include "Bit_math.h"
#include "UART.h"


#define MP3_CMD_SIZE       10

#define MP3_START_BYTE      0x7E
#define MP3_START_VERSION   0xFF
#define MP3_NO_FEEDBACK     0x00
#define MP3_REQ_FEEDBACK    0x01
#define MP3_END_BYTE        0xEF


/*COMMANDS*/
#define MP3_PLAY_BACK       0x03
#define MP3_SET_VOLUME      0x06
#define MP3_RESET           0x0C
#define MP3_PLAY            0x0D
#define MP3_PAUSE           0x0E





u8 Mp3_Cmd_Default[MP3_CMD_SIZE]={MP3_START_BYTE,
		                          MP3_START_VERSION,
								  6,
								  0,
								  MP3_NO_FEEDBACK,
								  0,
								  0,
								  0,
								  0,
								  MP3_END_BYTE};

static void Send_Cmd_Frame(void)
{
	u8 Check_Sum=0;
	u8 Add_Frame=0;
	u8 i;
	for (i = 0; i < 10; i++)
	{

		if (i>0 && i<7)
		{
			Add_Frame+=Mp3_Cmd_Default[i];
		}
		else if (i==7)
		{

			Check_Sum=0XFFFF-Add_Frame+1;
			Mp3_Cmd_Default[7]=Check_Sum>>8;
			Mp3_Cmd_Default[8]=(u8)Check_Sum;
		}
		USART_Send_Byte(Mp3_Cmd_Default[i]);
	}
}


void Mp3_Init(void)
{
/*ENABLE USART CONFIGURATIONS DURING RUNTIME*/
UART0.BAUD_RATE=9600;
UART0.ENABLE_MODE=USART_TX_ONLY;
UART0.COMMUNICATION_TYPE=UART_ASYNCH_DOUBLESPEED;
UART0.COMMUNICATION_MODE=UART_SINGLE_PROCESSOR;
UART0.CONTROL_FRAME=USART_PARITY_DISABLED_1_STOP;
UART0.DATA_FRAME=USART_8_BIT_DATA;
UART0.INTERRUPT_SOURCE=USART_INTERRUPT_DISABLE;

/*INTIAT USART*/
USART_Init();

Mp3_Cmd_Default[3]=MP3_RESET;
Send_Cmd_Frame();
Mp3_Cmd_Default[3]=0;

_delay_ms(1000);

}


void Mp3_PlayTrack(u16 TrackNumber)
{
Mp3_Cmd_Default[3]=MP3_PLAY_BACK;
Mp3_Cmd_Default[5]=TrackNumber>>8;
Mp3_Cmd_Default[6]=(u8)MP3_PLAY_BACK;
Send_Cmd_Frame();
Mp3_Cmd_Default[3]=0;
Mp3_Cmd_Default[5]=0;
Mp3_Cmd_Default[6]=0;
}


void Mp3_SetVolume(u16 Volume_Level)
{
Mp3_Cmd_Default[3]=MP3_SET_VOLUME;
Mp3_Cmd_Default[5]=Volume_Level>>8;
Mp3_Cmd_Default[6]=(u8)MP3_PLAY_BACK;
Send_Cmd_Frame();
Mp3_Cmd_Default[3]=0;
Mp3_Cmd_Default[5]=0;
Mp3_Cmd_Default[6]=0;
}
