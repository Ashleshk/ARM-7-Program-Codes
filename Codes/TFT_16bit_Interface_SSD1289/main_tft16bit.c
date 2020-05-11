

#include <lpc214x.h>
#include "TFT16bit_LCD.h"

void  delay_ms(unsigned int time)	   //This Function is used to cause delay between LED ON and OFF events
{  
 unsigned int  i, j;
 for (j=0; j<time; j++)
  for(i=0; i<8002; i++);
}

void LCD_color(unsigned int val)
{	
	unsigned int i;
	WriteReg(0x4E, 0); 		//Set X-position 0    
	WriteReg(0x4F, 0);	   	//Set Y-position 0 

	WriteIndex(0x0022);					  //Select GRAM

	IOCLR0	= TFT_CS;									//Chip Select low
	IOSET0	= TFT_WR;
	IOSET0	= TFT_RD;
	IOSET0	= TFT_RS;

	for(i=0; i<(320*240); i++)						//write  bit color value to the GRAM
	{
		 WriteWord(val);
		// WriteByte(val>>8);
		// WriteByte(val & 0xFF); 
		 //delay(500);
	}
	delay(10);
	IOSET0	= TFT_CS;								  	//Chip Select high
}


int main()
{
	
	LCD_initialise();
	LCD_ON ();
	
	
	
	while(1)
	{
		LCD_color(Green);
		delay_ms(100);
		LCD_color(Red);
		delay_ms(100);
		LCD_color(Blue);
		delay_ms(100);
		LCD_color(Grey);
		delay_ms(100);
		LCD_color(Green);
		delay_ms(100);
		LCD_color(Black);
		delay_ms(100);
		LCD_color(Blue2);
		delay_ms(100);
		LCD_color(Yellow);
		delay_ms(100);
		LCD_color(Cyan);
		delay_ms(100);
		LCD_color(Magenta);
		delay_ms(1000);
	}

}
