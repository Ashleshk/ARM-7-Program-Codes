# include<lpc21xx.h>
# define GLCD_CS1_SET IOSET1= 1<<27
# define GLCD_CS2_SET IOSET1= 1<<28
# define GLCD_RST_SET IOSET1= 1<<29
# define GLCD_CS1_CLR IOCLR1= 1<<27
# define GLCD_CS2_CLR IOCLR1= 1<<28
# define GLCD_RST_CLR IOCLR1= 1<<29  
# define RS_CMD  IOCLR1= 1<<24
# define RS_DATA IOSET1= 1<<24
# define write   IOCLR1= 1<<25
# define en_high   IOSET1= 1<<26
# define en_low    IOCLR1= 1<<26

void ms_delay(unsigned char t)
{
	int i, j;
	for(i=0;i<t;i++)
	{
		for(j=0;j<8002;j++);
	}
}

void GLCD_CMD( unsigned char val)
{

	RS_CMD;
 	write;
	IOCLR1=0x00FF0000;
	IOSET1=(val<<16);
 	en_high;
	ms_delay(1);
	en_low;
}

void GLCD_INIT()
{
	GLCD_RST_SET;
	GLCD_CS1_SET;
	GLCD_CS2_SET;
	GLCD_CMD(0x3F);
	GLCD_CMD(0xB8);
	GLCD_CMD(0x40);
}

void PAGE_COL_SEL( unsigned char pag,unsigned char col)
{
	if(col<64)
	{
		GLCD_CS1_SET;
		GLCD_CS2_CLR;
		GLCD_CMD(0x40|col);
	 	GLCD_CMD(0xB8|pag);
	}
	else
	{   
		GLCD_CS1_CLR;
		GLCD_CS2_SET;
		GLCD_CMD(0x40|(col-64));
	 	GLCD_CMD(0xB8|pag);
	}
}

void GLCD_DATA( unsigned char val)
{
	RS_DATA;
	write;
	IOCLR1=0x00ff0000;
	IOSET1=(val<<16);  
	en_high;
	ms_delay(1);
	en_low;
}
void glcd_clr()
{	
	int i,j;
	for (i=0;i<128;i++)
	{
		for (j=0;j<8;j++)
		{
		   	PAGE_COL_SEL(j,i);
			GLCD_DATA(0x00);
		}
	}
}

int main()
{
	int i,j,k=0;
		unsigned char x[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0x60,0x60,0x30,0x30,0x10,0x18,0x08,0x0C,0x04,0x04,0x02,0x02,0x00,0x00,0x00,0x00,0x00,
	 	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xE0,0xF0,0xF0,0xF8,0xFC,0xFC,0xFE,0xFE,0x7F,0x0F,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00	,
	 	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0x70,0x78,0x3C,0x3E,0x3F,0x3F,0x1F,0x1F,0x1F,0x0F,0x0F,0x0F,0x07,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00	,
	 	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xE0,0xE0,0xF0,0xF0,0xF8,0xF8,0xFC,0xFC,0xFC,0xFE,0xFE,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x7F,0x7F,0x7F,0x7F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x9F,0x9F,0x9F,0x9F,0x9E,0x9E,0x9C,0x98,0x98,0x8C,0x86,0x06,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00	,
	 	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFE,0xFF,0xFF,0xDF,0xDF,0xDF,0xDF,0xEF,0xEF,0xEF,0xEF,0xE7,0xF7,0xF7,0xF3,0xF3,0xF3,0xFB,0xF9,0xF9,0xF9,0xFD,0xFC,0xFC,0xFC,0xFC,0xFC,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x3F,0x1F,0x0F,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00	 ,
	 	0x02,0x02,0x06,0x06,0x06,0x06,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x07,0x07,0x07,0x07,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00	,
	 	0x00,0xF8,0xF8,0xF8,0xF8,0x90,0x08,0x98,0xF8,0xF8,0xF0,0x70,0x00,0xF8,0xFA,0xFB,0xFA,0xF8,0x00,0xF0,0xF8,0xF8,0xF8,0x88,0x08,0x28,0x38,0xB8,0xB0,0x00,0x00,0xF0,0xF8,0xF8,0xF8,0x88,0x88,0x88,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xF8,0xF8,0xF8,0x90,0x08,0x98,0xF8,0xF8,0xF8,0x70,0x00,0x00,0xFA,0xFB,0xFB,0xFA,0x00,0x00,0xF8,0xF8,0xF8,0x90,0x18,0x08,0x98,0xF8,0xF8,0xF0,0x20,0x00,0xF0,0xF8,0xF8,0xF8,0x88,0x08,0x38,0x38,0xB8,0xB0,0x00,0x00,0xF8,0xF8,0xF8,0xF0,0x10,0x18,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 	0x40,0x07,0x07,0x47,0x07,0x01,0x41,0x41,0x01,0x20,0x00,0x40,0x00,0x00,0x01,0x41,0x41,0x00,0x00,0x00,0x40,0x40,0x01,0x41,0x01,0x01,0x01,0x41,0x01,0x00,0x00,0x40,0x40,0x01,0x41,0x41,0x01,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x40,0x40,0x00,0x40,0x07,0x07,0xC7,0x07,0x01,0x01,0x01,0x01,0x41,0x40,0x00,0x40,0x40,0x01,0x41,0x01,0x01,0x00,0x40,0x07,0x07,0x07,0x41,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x40,0x00,0x00,0x01,0x01,0x41,0x01,0x41,0x41,0x01,0x40,0x40,0x00,0x01,0x01,0x41,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

	
	};
	PINSEL1= 0x00000000;
	PINSEL2= 0x00000000;
	 
	IODIR1= 0XFFFF0000;

	GLCD_INIT();
	glcd_clr();
  
	for (j=0;j<8;j++)
  	{
		 for(i=0;i<128;i++)
		 {
		 	if(i<=7 ||i>=120 ||(i>=16 && i<=23)||(i<=111 &&i>=103))
			{
			PAGE_COL_SEL(j,i);
			GLCD_DATA(0x0FF);
			}
			if(j==7||j==0 )
			{
			PAGE_COL_SEL(j,i);
			GLCD_DATA(0x0FF);
			}
			if(j==2 || j==5)
			{
			   if(i>=16 && i<=111) {
			PAGE_COL_SEL(j,i);
			GLCD_DATA(0x0FF);
			}
			} 
   			
			//k=k+1;
		 }
	}
	

}
	



