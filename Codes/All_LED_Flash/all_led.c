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
 IODIR1 = 0xFFFF << 16 ;		//Configure P1.16- P1.23 as output
 IODIR0 = 0xFFFFFFFF  ;
 while(1)	
 { 
							//Initial Value
		
				IOSET1 = 0xFFFF << 16; //Turn ON LED
				IOSET0 = 0xFFFFFFFF  ;
				delay_ms (250);
				IOCLR1 = 0xFFFF << 16;	//Turn OFF LED
				IOCLR0 = 0xFFFFFFFF  ;
				delay_ms (250);
							//Shift the value
			
 }

}