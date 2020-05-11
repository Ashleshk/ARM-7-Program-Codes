/**************************************************************************************************
Expt. 1b.: Interfacing 128x64 GLCD to LPC2148
Platform: Smart Logic LPC2148 Development Board.
		
Clock Settings:
	FOSC	>>	12MHz (onboard)
	PLL		>>	M=5, P=2
	CCLK	>>  60MHz
	PCLK	>>  15MHz 
		
Hardware Setup:-
	LCD data pin :-P0.16-P0.23 
	RS	-	P1.16
	RW	-	P1.17
	EN	-	P1.18
	CS1 - P1.19
	CS2	- P1.20
	RST - P1.21
	
********************************************************************************/

//Include Controller specific header file
#include <lpc214x.h> 
#include "image.h"
#include "glcd_font_8x8.h"

//Define Macro Functions
#define GLCD_CS1_SET() 	IOSET1=1<<19;
#define	GLCD_CS2_SET()	IOSET1=1<<20;
#define GLCD_RST_SET()	IOSET1=1<<21;
#define GLCD_CS1_CLR() 	IOCLR1=1<<19;
#define	GLCD_CS2_CLR()	IOCLR1=1<<20;
#define GLCD_RST_CLR()	IOCLR1=1<<21;
unsigned char c=0;

//Function to generate software delay
//Calibrated to 1ms
void  delay_ms(unsigned int time)	   
{  
 unsigned int  i, j;
 for (j=0; j<time; j++)
  for(i=0; i<8002; i++);
}

void GLCD_command(unsigned char command)
{
	IOCLR0 = 0xFF<<16;	// Clear LCD Data lines
	IOCLR1=1<<16;		 		// RS=0 for command
	IOCLR1=1<<17;		 		// RW=0 for write
	IOSET0=command<<16;	// put command on data line
	IOSET1=(1<<18);			// en=1 
	delay_ms(1) ;			// delay
	IOCLR1=(1<<18);	 		// en=0
}

void GLCD_data(unsigned char data)
{
	IOCLR0 = 0xFF<<16;	// Clear LCD Data lines
	IOSET1=1<<16;		 		// RS=1 for data
	IOCLR1=1<<17;		 		// RW=0 for write
	IOSET0= data<<16; 	// put command on data line
	IOSET1=(1<<18);			//en=1 
	delay_ms(1) ;	  	//delay
	IOCLR1=(1<<18);			//en=0
 }


void GLCD_init()
{
	GLCD_RST_SET();			//Set the Reset Pin
	GLCD_CS1_SET();			//Select Chip1
	GLCD_CS2_SET();			//Select Chip2
	GLCD_command(0x3F);	//GLCD_Display_ON_Command
	delay_ms(10) ;			//Delay
	GLCD_command(0xB8); 	//Set Page Address to 0
	delay_ms(10) ;			//Delay
	GLCD_command(0x40); 	//Set Coloumn Address to 0
	delay_ms(10) ;			//Delay
}

void GLCD_setpage(unsigned char page)
{
	GLCD_CS1_SET();			//Select Chip1
	GLCD_CS2_SET();			//Select Chip2
	GLCD_command(0xB8|page);	//GLCD_pageset_Command
	delay_ms(10) ;			//Delay
}

void GLCD_setcolumn(unsigned char column)
{
	if(column<64)
	{
		c=column;
		GLCD_CS1_SET();			//Select Chip1
		GLCD_CS2_CLR();			//Select Chip2
		GLCD_command(0x40|column);	//GLCD_columnset_Command
		delay_ms(10);
	}
	else
	{ 	
		c=column;
		GLCD_CS1_CLR();			//Select Chip1
		GLCD_CS2_SET();			//Select Chip2
		GLCD_command(0x40|column-64);	
		delay_ms(10);
	}
}
 

GLCD_data_array (const unsigned char *value,unsigned int limit)
{
	unsigned int i;
	for(i=0;i<limit;i++)
	{
		if(c<64)
		{
			GLCD_CS1_SET();
			GLCD_CS2_CLR();
			GLCD_data(value[i]);
			c++;
		}
		else
		{
			GLCD_setcolumn(c);
			GLCD_CS1_CLR();
			GLCD_CS2_SET();
			GLCD_data(value[i]);
			c++;
		}
		if(c>127)
		return;
	}
}

void putstr_font1(unsigned char y,unsigned char x,unsigned char *str)
{	
	unsigned char i;	
	unsigned int a;	
	GLCD_setcolumn(y);	
	GLCD_setpage(x);	
	for(i=0;str[i]!=0;i++)	
		{		
			a=(*(str+i));		
			a*=8;
			GLCD_data_array(&FONT1_8x8[a],8);	
		}
}

void GLCD_Image_Display(unsigned char *image)
{
	int i,j;
	for(i=0;i<8;i++)
	{
		GLCD_setpage(i);
		GLCD_setcolumn(0);
		for(j=0;j<64;j++)	//Send data to first half
			GLCD_data(image[(i*128)+j]);
				
		GLCD_setcolumn(64);
		for(j=64;j<128;j++)	//Send data to second half
			GLCD_data(image[(i*128)+j]);
	}
}

void GLCD_show_image(unsigned char *image)
{
	int i,j;
	int k=0; //To access element from Image Array
	for(i=0;i<8;i++)
	{
		GLCD_setpage(i);
		GLCD_setcolumn(0);
		for(j=0;j<64;j++)	//Send data to first half
			{
				GLCD_data(image[k]);
				k++;
			}
				
		GLCD_setcolumn(64);
		for(j=0;j<64;j++)	//Send data to second half
			{
				GLCD_data(image[k]);
				k++;
			}
	}
}


void GLCD_Clear_Display()
{
	int i,j;
	for(i=0;i<8;i++)
	{
		GLCD_setpage(i);
		GLCD_setcolumn(0);
		for(j=0;j<64;j++)	//Send data to first half
			GLCD_data(0x00);
				
		GLCD_setcolumn(64);
		for(j=0;j<64;j++)	//Send data to second half
			GLCD_data(0x00);
	}
}



int main(void)
{
	unsigned char str1[]="  SMART LOGIC   ";
	unsigned char str2[]="  TECHNOLOGIES  ";
	PINSEL1 = 0x00;		//Configure PORT0 as GPIO
	PINSEL2 = 0X00;		//Configure PORT1 as GPIO
	IODIR1= 0xFF<<16;	//Configure P1.23 - P1.16 as output
	IODIR0=	0xFF<<16; //Configure P0.23 - P0.16 as output
	GLCD_init();				//Initialize GLCD 128x64
	GLCD_Clear_Display();
	putstr_font1(0,1,str1);
	putstr_font1(0,2,str2);
	delay_ms(500);
	GLCD_Image_Display(SLT_IMG);
	while(1);
}