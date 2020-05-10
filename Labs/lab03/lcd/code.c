#include<lpc214x.h>
void LCD_init();

void LCD_command(unsigned char cmd);

void LCD_data(unsigned char data);

void LCD_write_string(unsigned char *str);

void ms_delay(int k);	  

int main()
{
	unsigned char var1[]="WELCOME";
	unsigned char var2[]="PICT";
	PINSEL0=0x00000000;
	PINSEL1=0x00000000;
	PINSEL2=0X00000000;
	IODIR0=0xFFFFFFFF;
	IODIR1=0xFFFFFFFF;
	IODIR0=0x0FF<<16;			// 8 data lines for lcd
	IODIR1=0X07<<17;		   // for rs r/rw en

	LCD_init();
	LCD_write_string(var1);
	LCD_command(0xC0);
	LCD_write_string(var2);
    return 0;
}
void ms_delay(int k)	  
{
int i,j;
	for(i=0;i<k;i++)
	{
		for(j=0;j<1008;j++)
		{
		}
	}
}
void LCD_init()
{
   LCD_command(0x38);
   ms_delay(100);
   LCD_command(0x01);
   ms_delay(100);
   LCD_command(0x0E);
   ms_delay(100);
   LCD_command(0x06);
   ms_delay(100);
   LCD_command(0x80);
   ms_delay(100);
   
   
}
void LCD_command(unsigned char cmd)	 
{
   IOCLR0=0xFF<<16;
   IOCLR1=1<<17;
   IOCLR1=1<<18;
   IOSET0=cmd<<16;
   IOSET1=1<<19;
   ms_delay(10);
   IOCLR1=1<<19;
}
void LCD_data(unsigned char data)
{
   IOCLR0=0xFF<<16;
   IOSET1=1<<17;
   IOCLR1=1<<18;
   IOSET0=data<<16;
   IOSET1=1<<19;
   ms_delay(10);
   IOCLR1=1<<19;
}
void  LCD_write_string(unsigned char *str)
{
	  int i=0;
	  while(str[i] !='\0')
	  {
	     LCD_data(str[i]);
		 ms_delay(100);
		 i++;
	  }


}