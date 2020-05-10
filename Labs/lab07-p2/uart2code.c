#include<lpc214x.h>			    //header file declaration
 unsigned char val;

void UART0_INIT()				//uart)_ init
{
	U0LCR = 0x9F;				// 8 bit,1stop bit,even parity
	U0DLL = 0X61;				// 9600 baud rate -lower
	U0DLM = 0X00;			    //                -higher
	U0LCR = 0X1F;
} 

unsigned char rxbyte(void)	    //rxbyte -- recive byte
{  
    while(!(U0LSR & 0X01));
	{
		val = U0RBR;
		return val;
		
	}
}
void txbyte(unsigned char d)	//txbyte -- transmit byte
{
	while(!(U0LSR &0x40));
	
		U0THR = d;
	
}
void UART0rectran_string(unsigned char *a)  //transmit string byte-by-byte
{
	int i =0;
	while(a[i] != '\0')
	{
		txbyte(a[i]);
		i++;
	}

}
void UART0Tran(void)					//recive bunch of char and transmit at once
{
	int i=0;
	unsigned char arr[10]="";
	for(i=0;i<10;i++)
	{
		arr[i]=rxbyte();
	}
	UART0rectran_string(arr);

}
int main()							  // main module
{
	unsigned char var1[]={"\n\rWELCOME\n\r"};
	unsigned char var2[]={"\n\rPICT\n\r"};
	 
    PINSEL0 =0x00000005;			 // p0.0 AND P0.1 as UART0 tx & rx
	UART0_INIT();
	UART0rectran_string(var1);
	UART0rectran_string(var2);
	UART0rectran_string("\n");
	UART0Tran();
	return 0;
}

                                                                             