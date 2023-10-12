/*
 * UART.c
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

#define NULL (void*)0

USART_CONFIGURATION_STRUCT   UART0={0,
		                            .RXC_INERRUPT_CB=NULL,
									.TXC_INERRUPT_CB=NULL,
									.UDRE_INERRUPT_CB=NULL};


ISR(USART_RXC_vect)
{
	if(UART0.RXC_INERRUPT_CB !=NULL)
	{
		UART0.RXC_INERRUPT_CB();
	}
	else {
		//ERROR HANDLING
	}

}


ISR(USART_TXC_vect)
{
	if(UART0.TXC_INERRUPT_CB !=NULL)
	{
		UART0.TXC_INERRUPT_CB();
	}
	else {
		//ERROR HANDLING
	}
}


ISR(USART_UDRE_vect)
{
	if(UART0.UDRE_INERRUPT_CB !=NULL)
	{
		UART0.UDRE_INERRUPT_CB();
	}
	else {
		//ERROR HANDLING
	}
}



void USART_Init(void)
{
u8 UCSRA_Temp=0;
u8 UCSRB_Temp=0;
u8 UCSRC_Temp=0;
f32 UBRR_Temp=0;

switch(UART0.ENABLE_MODE)
{
case USART_DISABLE:
break;
case USART_TX_ONLY:
UCSRB_Temp|=(1<<TXEN);
break;
case USART_RX_ONLY:
UCSRB_Temp|=(1<<RXEN);
break;
case USART_TX_RX  :
UCSRB_Temp|=(1<<RXEN)|(1<<TXEN);
break;
default:
break;
}

switch(UART0.COMMUNICATION_TYPE)
{

case UART_ASYNCH_NORMALSPEED:
UBRR_Temp=(((f32)F_CPU)/(16.0*UART0.BAUD_RATE))-0.5;
break;
case UART_ASYNCH_DOUBLESPEED:
UCSRA_Temp|=(1<<U2X);
UBRR_Temp=(((f32)F_CPU)/(8.0*UART0.BAUD_RATE))-0.5;
break;
case USART_SYNCH:
UCSRC_Temp|=(1<<UMSEL);
UBRR_Temp=(((f32)F_CPU)/(2.0*UART0.BAUD_RATE))-0.5;

switch(UART0.CLOCK_PLARITY)
{
case USART_SAMPLE_ON_FALLING:
break;
case USART_SAMPLE_ON_RISING:
UCSRC_Temp|=(1<<UCPOL);
break;
default:
break;
}

break;
default:
break;
}


switch(UART0.COMMUNICATION_MODE)
{
case UART_SINGLE_PROCESSOR:
break;
case UART_MULTI_PROCESSOR:
UCSRA_Temp|=(1<<MPCM);
break;
default:
break;
}


switch(UART0.INTERRUPT_SOURCE)
{
case USART_INTERRUPT_DISABLE:
break;
case USART_RXCOMP_ENABLE:
UCSRB_Temp|=(1<<RXCIE);
break;
case USART_TXCOMP_ENABLE:
UCSRB_Temp|=(1<<TXCIE);
break;
case USART_UDR_EMPTY_ENABLE:
UCSRB_Temp|=(1<<UDRE);
break;
case USART_RXCOMP_TXCOMP_ENABLE:
UCSRB_Temp|=(1<<RXCIE)|(1<<TXCIE);
break;
case USART_RXCOMP_UDR_EMPTY_ENABLE:
UCSRB_Temp|=(1<<RXCIE)|(1<<UDRE);
break;
case USART_TXCOMP_UDR_EMPTY_ENABLE:
UCSRB_Temp|=(1<<TXCIE)|(1<<UDRE);
break;
case USART_RXCOMP_TXCOMP_UDR_EMPTY_ENABLE:
UCSRB_Temp|=(1<<RXCIE)|(1<<UDRE)|(1<<TXCIE);
break;
default:
break;
}


switch(UART0.DATA_FRAME)
{
case USART_5_BIT_DATA:
break;
case USART_6_BIT_DATA:
UCSRC_Temp|=(1<<UCSZ0);
break;
case USART_7_BIT_DATA:
UCSRC_Temp|=(1<<UCSZ1);
break;
case USART_8_BIT_DATA:
UCSRC_Temp|=(1<<UCSZ0)|(1<<UCSZ1);
break;
case USART_9_BIT_DATA:
UCSRC_Temp|=(1<<UCSZ0)|(1<<UCSZ1);
UCSRB_Temp|=(1<<UCSZ2);
break;
default:
break;
}



switch(UART0.CONTROL_FRAME)
{
case USART_PARITY_DISABLED_1_STOP:
break;
case USART_PARITY_DISABLED_2_STOP:
UCSRC_Temp|=(1<<USBS);
break;
case USART_PARITY_EVEN_1_STOP:
UCSRC_Temp|=(1<<UPM1);
break;
case USART_PARITY_EVEN_2_STOP:
UCSRC_Temp|=(1<<USBS)|(1<<UPM1);
break;
case USART_PARITY_ODD_1_STOP:
UCSRC_Temp|=(1<<UPM1)|(1<<UPM0);
break;
case USART_PARITY_ODD_2_STOP:
UCSRC_Temp|=(1<<USBS)|(1<<UPM1)|(1<<UPM0);
break;
default:
break;
}



UBRRH=((unsigned short)UBRR_Temp)>>8;
UBRRL=((u8)UBRR_Temp);
UCSRA=UCSRA_Temp;
UCSRC=UCSRC_Temp;
UCSRB=UCSRB_Temp;
}




void USART_Send_Byte(u8 Data)
{

while(GET_BIT(UCSRA,UDRE)==0);
if (UART0.DATA_FRAME==USART_9_BIT_DATA)
{
UCSRB|=(UCSRB & 0XFE)|(Data&(1<<8)>>8);
}
UDR=(u8)Data;

}




u16 USART_Read_Byte(void)
{
u16 Data;
while(GET_BIT(UCSRA,RXC)==0);
if(UCSRA & ((1<<FE) & (1<<DOR) & (1<<PE)))
{
	if(UCSRA & ((1<<FE)))
	{

		UART0.READ_ERROR=USART_FRAME_ERROR;
	}
	else if (UCSRA & ((1<<DOR)))
	{
		UART0.READ_ERROR=USART_DOR_ERROR;
	}
	else
	{
		UART0.READ_ERROR=USART_PARITY_ERROR;
	}
}

if(UART0.DATA_FRAME==USART_9_BIT_DATA)
{
Data=UCSRB & (1<<RXB8)<<7;
}
Data|=UDR;
return Data;
}

