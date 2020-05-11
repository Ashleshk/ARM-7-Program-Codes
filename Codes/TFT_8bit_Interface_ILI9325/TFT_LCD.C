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



void delay(unsigned int time)    
{ 
	unsigned int i,j; 
	for(i=0; i<time; i++ )
		for(j=0; j<1000; j++);
}


void WriteByte(unsigned char data)	 //write and read a byte at the same time (full duplex operation)
{
	IOCLR0  = 0xFF	<<16;
	IOSET0	= data <<16;
	IOCLR1 	= TFT_WR;   //Writing
	//delay(1);	
	IOSET1	= TFT_WR;   //Writing 
}

void WriteIndex(unsigned int index)				//select a register in the ILI9325 chip
{
	IOCLR1	= TFT_CS;		//Chip Select LOW
	IOSET1	= TFT_RD;    
	IOCLR1	= TFT_RS;	//Command
	WriteByte(0);
	WriteByte(index);
	IOSET1	= TFT_CS;								   	//Chip Select high
}

void WriteData(unsigned int data)
{
	IOCLR1	= TFT_CS;		//Chip Select LOW
	IOSET1	= TFT_RD;    
	IOSET1	= TFT_RS;	//data
	WriteByte((data >> 8)& 0xFF);
	WriteByte(data & 0xFF);
	IOSET1	= TFT_CS;									//Chip Select high
}



void WriteReg(unsigned int reg, unsigned int value)	   	//write value to a register in LCD
{														//write reg = write index + write data at that index
	WriteIndex(reg);
	WriteData(value);
}


void LCD_initialise(void)
{
	IODIR0 	|= 0xFF <<16;
	IODIR1	|= 0xFFFF0000;
	
	IOSET1	= TFT_RST;
	delay(75);
	IOCLR1	= TFT_RST;
	delay(75);
	IOSET1	= TFT_RST;
	delay(75);

		WriteReg(0xB1, 0x0000);
		WriteReg(0xE5, 0x78F0); /* set SRAM internal timing */
		WriteReg(0x01, 0x0100); /* set Driver Output Control */
		WriteReg(0x02, 0x0700); /* set 1 line inversion */
		WriteReg(0x03, 0x1030); /* set GRAM write direction and BGR=1 */
	WriteReg(0x04, 0x0000); /* Resize register */
	WriteReg(0x08, 0x0207); /* set the back porch and front porch */
	WriteReg(0x09, 0x0000); /* set non-display area refresh cycle ISC[3:0] */
	WriteReg(0x0A, 0x0000); /* FMARK function */
	WriteReg(0x0C, 0x0000); /* RGB interface setting */
	WriteReg(0x0D, 0x0000); /* Frame marker Position */
	WriteReg(0x0F, 0x0000); /* RGB interface polarity */
	/*************Power On sequence ****************/
 	WriteReg(0x10, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
 	WriteReg(0x11, 0x0007); /* DC1[2:0], DC0[2:0], VC[2:0] */
 	WriteReg(0x12, 0x0000); /* VREG1OUT voltage */
 	WriteReg(0x13, 0x0000); /* VDV[4:0] for VCOM amplitude */
 	WriteReg(0x07, 0x0001);
 	delay(200); 
	/* Dis-charge capacitor power voltage */
	WriteReg(0x10, 0x1090); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
	WriteReg(0x11, 0x0227); /* Set DC1[2:0], DC0[2:0], VC[2:0] */
	delay(50); 							/* Delay 50ms */
	WriteReg(0x12, 0x001F); 
	delay(50); 							/* Delay 50ms */
	WriteReg(0x13, 0x1500); /* VDV[4:0] for VCOM amplitude */
	WriteReg(0x29, 0x0027); /* 04 VCM[5:0] for VCOMH */
	WriteReg(0x2B, 0x000D); /* Set Frame Rate */
	delay(50); 							/* Delay 50ms */
	WriteReg(0x20, 0x0000); /* GRAM horizontal Address */
	WriteReg(0x21, 0x0000); /* GRAM Vertical Address */
	/* ----------- Adjust the Gamma Curve ---------- */
	WriteReg(0x30, 0x0000);
	WriteReg(0x31, 0x0707);
	WriteReg(0x32, 0x0307);
	WriteReg(0x35, 0x0200);
	WriteReg(0x36, 0x0008);
	WriteReg(0x37, 0x0004);
	WriteReg(0x38, 0x0000);
	WriteReg(0x39, 0x0707);
	WriteReg(0x3C, 0x0002);
	WriteReg(0x3D, 0x1D04);
	/* ------------------ Set GRAM area --------------- */
	WriteReg(0x50, 0x0000); /* Horizontal GRAM Start Address */
	WriteReg(0x51, 0x00EF); /* Horizontal GRAM End Address */
	WriteReg(0x52, 0x0000); /* Vertical GRAM Start Address */
	WriteReg(0x53, 0x013F); /* Vertical GRAM Start Address */
	
	WriteReg(0x60, 0xA700); /* Gate Scan Line */
	WriteReg(0x61, 0x0001); /* NDL,VLE, REV */
	WriteReg(0x6A, 0x0000); /* set scrolling line */
	/* -------------- Partial Display Control --------- */
	WriteReg(0x80, 0x0000);
	WriteReg(0x81, 0x0000);
	WriteReg(0x82, 0x0000);
	WriteReg(0x83, 0x0000);
	WriteReg(0x84, 0x0000);
	WriteReg(0x85, 0x0000);
	/* -------------- Panel Control ------------------- */
	WriteReg(0x90, 0x0010);
	WriteReg(0x92, 0x0600);
		WriteReg(0x07, 0x0133); /* 262K color and display ON */	
}


unsigned char ReadByte (void)
{
	unsigned char temp;
	
	IOCLR1 = TFT_RD;
	delay(1);
	temp = (IOPIN0 & 0xFF0000) >> 16;
	IOSET1 = TFT_RD;
	return temp;
}

unsigned int ReadData(void)
{
unsigned int temp = 0;
	IODIR0 = 0x00<<16;	
	IOCLR1	= TFT_CS;		//Chip Select LOW
	IOSET1	= TFT_RD;    
	IOSET1	= TFT_RS;	//data
	temp 		= ReadByte()<<8;					   	//read higher byte
	temp 	 |= ReadByte();
	IOSET1	= TFT_CS; 									//Chip Select high
	IODIR0 = 0xFF<<16;	
	return temp;
}



