/* LPC1768 TFT LCD interface
		* TFT_Data	: P0.16-P0.23
		* TFT_RESET	: P1.16
		* TFT_CS		:	P1.17
		* TFT_RS		: P1.18 	
		* TFT_WR  	: P1.19
		* TFT_RD		: P1.20
*/

#include <lpc214x.h>
#include "TFT_LCD.h"

void  delay_ms(unsigned int time)	   //This Function is used to cause delay between LED ON and OFF events
{  
 unsigned int  i, j;
 for (j=0; j<time; j++)
  for(i=0; i<8002; i++);
}

void LCD_color(unsigned int val)
{	
	unsigned int i;
	WriteReg(0x20, 0); 		//Set X-position 0    
	WriteReg(0x21, 0);	   	//Set Y-position 0 

	WriteIndex(0x22);					  //Select GRAM

	IOCLR1	= TFT_CS;									//Chip Select low
	IOSET1	= TFT_WR;
	IOSET1	= TFT_RD;
	IOSET1	= TFT_RS;

	for(i=0; i<(320*240); i++)						//write  bit color value to the GRAM
	{
		 WriteByte(val>>8);
		 WriteByte(val & 0xFF); 
		 //delay(100);
	}
	delay(10);
	IOSET1	= TFT_CS;								  	//Chip Select high
}


int main()
{
	
	LCD_initialise();
	
	
	
	while(1)
	{
		LCD_color(White);
		delay_ms(500);
		LCD_color(Red);
		delay_ms(500);
		LCD_color(Blue);
		delay_ms(500);
		LCD_color(Grey);
		delay_ms(500);
		LCD_color(Green);
		delay_ms(500);
		LCD_color(Black);
		delay_ms(500);
		LCD_color(Blue2);
		delay_ms(500);
		LCD_color(Yellow);
		delay_ms(500);
		LCD_color(Cyan);
		delay_ms(500);
		LCD_color(Magenta);
		delay_ms(500);
	}

}
