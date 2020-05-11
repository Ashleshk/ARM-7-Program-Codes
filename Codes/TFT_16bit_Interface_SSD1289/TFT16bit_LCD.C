/* LPC1768 TFT LCD interface
		* TFT_Data	: P0.16-P0.23
		* TFT_RESET	: P1.16
		* TFT_CS		:	P1.17
		* TFT_RS		: P1.18 	
		* TFT_WR  	: P1.19
		* TFT_RD		: P1.20
*/

#include <lpc214x.h>
#include "TFT16bit_LCD.h"



void delay(unsigned int time)    
{ 
	unsigned int i,j; 
	for(i=0; i<time; i++ )
		for(j=0; j<1000; j++);
}


void WriteByte(unsigned char data)	 //write and read a byte at the same time (full duplex operation)
{
	IOCLR1  = 0xFF	<<16;
	IOSET1	= data <<16;
	IOCLR0 	= TFT_WR;   //Writing
	delay(1);	
	IOSET0	= TFT_WR;   //Writing 
}

void WriteWord(unsigned int data)	 //write and read a byte at the same time (full duplex operation)
{
	IOCLR1  = 0xFFFF0000;
	IOSET1	= data <<16;
	IOCLR0 	= TFT_WR;   //Writing
	//delay(1);	
	IOSET0	= TFT_WR;   //Writing
	//delay (500);
}

void WriteIndex(unsigned int index)				//select a register in the ILI9325 chip
{
	IOCLR0	= TFT_CS;		//Chip Select LOW
	IOSET0	= TFT_RD;    
	IOCLR0	= TFT_RS;	//Command
	//WriteByte(0);
	WriteWord(index);
	IOSET0	= TFT_CS;								   	//Chip Select high
}

void WriteData(unsigned int data)
{
	IOCLR0	= TFT_CS;		//Chip Select LOW
	IOSET0	= TFT_RD;    
	IOSET0	= TFT_RS;	//data
	//WriteByte((data >> 8)& 0xFF);
	//WriteByte(data & 0xFF);
	WriteWord (data);
	IOSET0	= TFT_CS;									//Chip Select high
}



void WriteReg(unsigned int reg, unsigned int value)	   	//write value to a register in LCD
{														//write reg = write index + write data at that index
	WriteIndex(reg);
	delay(500);
	WriteData(value);
}


void LCD_initialise(void)
{
	IODIR0 	|= 0xFF <<16;
	IODIR1	|= 0xFFFF0000;
	
	
	IOSET0	= TFT_RST;
	delay(10);
	IOCLR0	= TFT_RST;
	delay(10);
	IOSET0	= TFT_RST;
	delay(100);

		
	WriteReg(0x0007,0x0021);  //display control
	WriteReg(0x0000,0x0001);  //oscillator start 
	WriteReg(0x0007,0x0023);
	WriteReg(0x0003,0xAEAC); 
	WriteReg(0x000C,0x0007); 
	WriteReg(0x000D,0x000F); 
	WriteReg(0x000E,0x2900); 
	WriteReg(0x001E,0x00B3);
//	DelayMs(15);        
	WriteReg(0x0001,0x2b3F); 
	WriteReg(0x0002,0x0600); 
	WriteReg(0x0010,0x0000); 
	WriteReg(0x0011,0x60B0); // ID=11  AM=0
//	DelayMs(15);        
	WriteReg(0x0005,0x0000); 
	WriteReg(0x0006,0x0000); 
	WriteReg(0x0016,0xEF1C); 
	WriteReg(0x0017,0x0003); 
	WriteReg(0x0007,0x0233);
	WriteReg(0x000B,0x5312); 
	WriteReg(0x000F,0x0000);
//	DelayMs(15);         
	WriteReg(0x0041,0x0000); // scroll screen 1
	WriteReg(0x0042,0x0000); // scroll screen 2
	WriteReg(0x0048,0x0000); // first screen start
	WriteReg(0x0049,0x013F); // first screen end
	WriteReg(0x0044,0xEF00); 
	WriteReg(0x0045,0x0000); 
	WriteReg(0x0046,0x013F); 
	WriteReg(0x004A,0x0000); // second screen start
	WriteReg(0x004B,0x0000); // second screen end
//	DelayMs(15);       
	WriteReg(0x0030,0x0707); 
	WriteReg(0x0031,0x0704); 
	WriteReg(0x0032,0x0204); 
	WriteReg(0x0033,0x0201); 
	WriteReg(0x0034,0x0203);
	WriteReg(0x0035,0x0204);
	WriteReg(0x0036,0x0204);
	WriteReg(0x0037,0x0502);
	WriteReg(0x003A,0x0302);
	WriteReg(0x003B,0x0500);
	WriteReg(0x0023,0x0000);
	WriteReg(0x0024,0x0000);
	WriteReg(0x0025,0x8000); //ajout
//	DelayMs(20); 
	//WriteIndex(0x0022);
	//LCD_CS = 1;

}


unsigned char ReadByte (void)
{
	unsigned char temp;
	
	IOCLR0 = TFT_RD;
	delay(1);
	temp = (IOPIN0 & 0xFF0000) >> 16;
	IOSET0 = TFT_RD;
	return temp;
}

unsigned int ReadData(void)
{
unsigned int temp = 0;
	IODIR1 = 0x00000000;	
	IOCLR0	= TFT_CS;		//Chip Select LOW
	IOSET0	= TFT_RD;    
	IOSET0	= TFT_RS;	//data
	temp 		= ReadByte()<<8;					   	//read higher byte
	temp 	 |= ReadByte();
	IOSET0	= TFT_CS; 									//Chip Select high
	IODIR1 = 0xFFFF0000;	
	return temp;
}


void LCD_ON(void)
{ 
	// Display ON Setting
	WriteReg(0x0007,0x0021);  //display control
	WriteReg(0x0000,0x0001);  //oscillator start 
	WriteReg(0x0007,0x0023);
	WriteReg(0x0010,0x0000);
	delay(100); 
	WriteReg(0x0007,0x0033);
	WriteReg(0x0011,0x60B0); // ID=11  AM=0
	WriteReg(0x0002,0x0600);
	WriteIndex(0x0022);
}


