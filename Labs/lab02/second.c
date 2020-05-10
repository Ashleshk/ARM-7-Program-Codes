//        led chasing   R->L and L->R

#include <LPC214x.h>


void msdelay(int k)
{
int i,j;
	for(i=0;i<k;i++)
	{
		for(j=0;j<1008;j++)
		{
		}
	}
}

int main()
{
		unsigned int X;
		int i;
		PINSEL0=0x00000000;       //All are GPi/o
		PINSEL1=0x00000000;       //All are GPi/o
		PINSEL2=0x00000000;       //All are GPi/o
	 	IODIR0=0xFFFFFFFF;
		IODIR1=0xFFFFFFFF;
		IOSET0=0xFFFFFFFF;
		IOSET1=0xFFFFFFFF;

	

		while(1)
		{
		   	X=1<<15;
		   for(i=0;i<8;i++)
		   {
		   	  X=X<<1;
			  IOSET0=X;
			  msdelay(1000);
			  IOCLR0=X;
			  msdelay(1000);
		   }
		  
		  for(i=0;i<6;i++)
		   {
		   	  X=X>>1;
			  IOSET0=X;
			  msdelay(1000);
			  IOCLR0=X;
			  msdelay(1000);
		   }
		
	 	 }
		  
 return 0;
}

