#include<LPC214x.h>
void delay(int x);
void main()
{
     PINSEL1=0x00000000;
	 IODIR0=0x00FF0000;
	 while(1)
	 {
	     IOSET0 =0x00FF0000;
		 delay(500);
		 IOCLR0=0x00FF0000;
		 delay(500);
	 }
}
void delay(int x)
{
   int i ,j ;
   for (i=0;i<x;i++)
    for(j=0;j<1024;j++);
} 
 
