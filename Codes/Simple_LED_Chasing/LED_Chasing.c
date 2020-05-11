/**************************************************************************************************
		Company: Smart Logic Technologies
		Platform: LPC2148 Development Board.
    Clock Settings:
		FOSC	>>	12MHz (onboard)
		PLL		>>	M=5, P=2
		CCLK	>>  60MHz
		PCLK	>>  15MHz  	        
********************************************************************************/
//Include controller specific header file
#include  <lpc214x.h>				

//Function to generate delay
void  delay_ms(unsigned char time)	   //This Function is used to cause delay between LED ON and OFF events
{  
 unsigned int  i, j;
 for (j=0; j<time; j++)
  for(i=0; i<8002; i++);
}
                               

int  main(void)
{  
 int a, k;
 
 PINSEL2 = 0x00000000; 	//Configure PORT1 as GPIO
 IODIR1 = 0xFF << 16;		//Configure P1.16- P1.23 as output
 while(1)	
 { 
		a = 0x01;						//Initial Value
		for (k=0; k<8; k++)
			{
				IOSET1 = a << 16; //Turn ON LED
				delay_ms (250);
				IOCLR1 = a << 16;	//Turn OFF LED
				a = a << 1;				//Shift the value
			}
 }

}