#include <lpc214x.h>
unsigned char val ,X;

void UART0_INIT()
{
	U0LCR = 0x9F;
	U0DLL = 0X61;
	U0DLM = 0X00;
	U0LCR = 0X1F;
} 

unsigned char rxbyte()
{
    while(!(U0LSR &0X01));
	{
		val = U0RBR;
		return val;
		
	}
}
void txbyte(unsigned char d)
{
	while(!(U0LSR &0x40));
	{
		U0THR = d;
	}
}
void main()
{
    PINSEL0 =0x00000005;
	UART0_INIT();
	while(1)
	{
		X=rxbyte();
		txbyte(X);
	}
}

