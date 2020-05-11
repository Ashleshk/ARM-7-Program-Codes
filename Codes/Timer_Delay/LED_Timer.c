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

//Function to Intialize timer
void init_timer0 (void)
{
	T0CTCR = 0X00; 		//Configure as Timer
	T0PR = 15000 -1;	//Load Prescaler value
	T0TCR = 0X02;			//Reset the timer
}

void timer_delay (unsigned int time)
{
	T0TCR = 0X02;			//Reset the timer
	T0TCR = 0x01;			//Enable the timer
	while (T0TC < time);	//Check with desired time delay
	T0TCR = 0X02;			//Reset the timer
	T0TCR = 0x00;
	T0TC = 0x00; // to verify the simulation results
	T0PC = 0x00; // to verify the simulation results
}
int main (void)
{
	PINSEL2 = 0x00000000; 	//Configure PORT1 as GPIO
	init_timer0();					//Initialize the timer
	IODIR1 = 0xFF << 16;		//Configure P1.16 - P1.23 as output
	IOCLR1 = 0x00FF0000;		// set low
	 
	while (1)
	{
		IOSET1 = 0x00FF0000;   //set high
		timer_delay (100);
		IOCLR1 = 0x00FF0000;	 //set low
		timer_delay (100);
  }
}