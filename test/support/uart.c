#include <stdbool.h>

#include <xc.h>

#include "uart.h"

void uart_end(void);

void uart_start(void)
{
	PLLFBD = 63;
	CLKDIVbits.PLLPOST = 0;
	CLKDIVbits.PLLPRE = 1;
	
	while (!OSCCONbits.LOCK);
	
	__builtin_write_OSCCONL(OSCCONL & ~_OSCCON_IOLOCK_MASK);
	
	RPINR18bits.U1RXR = 6;
	RPOR3bits.RP7R = 3;
	
	__builtin_write_OSCCONL(OSCCONL | _OSCCON_IOLOCK_MASK);
	
	U1MODEbits.BRGH = 1;
	U1BRG = 86;
	
	U1MODEbits.UARTEN = 1;
	U1STAbits.UTXEN = 1;
}

void uart_stop(void)
{
	while (!U1STAbits.TRMT);
	
	U1STAbits.UTXEN = 0;
	U1MODEbits.UARTEN = 0;
	
	uart_end();
}

void uart_end(void)
{
	while (true);
}

int uart_putchar(int c)
{
	while (U1STAbits.UTXBF);
	U1TXREG = c;
	return c;
}
