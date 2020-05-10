#include<lpc214x.h>
int main()
{
	PINSEL0=0x00000000;       //All are GPi/o
		PINSEL1=0x00000000;       //All are GPi/o
		PINSEL2=0x00000000;       //All are GPi/o
	 	IODIR0=0xFFFFFFFF;
		IODIR1=0xFFFFFFFF;
		IOSET0=0xFFFFFFFF;
		IOSET1=0xFFFFFFFF;
		return 0;
}