#include<lpc214x.h>

void LCD_init();
void LCD_cmd(unsigned char cmd);
void LCD_data(unsigned char data);
void  LCD_Write_string(unsigned char *str);
void ms_delay(int k);

void UART1_init(void);
unsigned char UART1_Rxbyte(void);
int main()
{
	unsigned char flag;
	int i;
	unsigned char stg[]="GPGGA",dummy;
	unsigned char cmd_data[5],longitude[12],latitude[12],timestamp[10];
	PINSEL0=0X00000000;
	PINSEL1=0X00000000;
	PINSEL2=0X00000000;
	IODIR0=0x000000FF;
	IODIR1=0xFF<<16;

	LCD_init();
	UART1_init();
	LCD_Write_string("GPS Data");
	while(1)
	{
		if(UART1_Rxbyte()=='$')
		{
			for(i=0;i<5;i++)
			{
				flag=0;
				cmd_data[i] = UART1_Rxbyte();
				if(cmd_data[i] !=stg[i])
				{
					flag=1;
					break;
				}
			}
			dummy=UART1_Rxbyte();
			if(flag==0)
			{
				for(i=0;i<10;i++)
				{
					timestamp[i]=UART1_Rxbyte();
				}
				dummy=UART1_Rxbyte();
				for(i=0;i<12;i++)
				{
					latitude[i]=UART1_Rxbyte();

				}
				dummy=UART1_Rxbyte();
				for(i=0;i<12;i++)
				{
					longitude[i]=UART1_Rxbyte();

				}
			}
			LCD_cmd(0x01);
			ms_delay(100);
			LCD_Write_string("Time Stamp :");
			LCD_cmd(0xC0);
			ms_delay(100);
			LCD_Write_string(timestamp);
			LCD_cmd(0x01);
			ms_delay(100);
			LCD_Write_string("longitude:");
			LCD_cmd(0xC0);
			ms_delay(100);
			LCD_Write_string(latitude);
			LCD_cmd(0x01);
			ms_delay(100);
			LCD_Write_string("latitude :");
			LCD_cmd(0xC0);
			ms_delay(100);
			LCD_Write_string(longitude);

		}
	}

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
   LCD_cmd(0x38);
   ms_delay(100);
   LCD_cmd(0x01);
   ms_delay(100);
   LCD_cmd(0x0E);
   ms_delay(100);
   LCD_cmd(0x06);
   ms_delay(100);
   LCD_cmd(0x80);
   ms_delay(100);
   
   
}
void LCD_cmd(unsigned char cmd)	 
{
   IOCLR0=0xFF<<16;
   IOCLR1=1<<16;
   IOCLR1=1<<17;
   IOSET0=cmd<<16;
   IOSET1=1<<18;
   ms_delay(10);
   IOCLR1=1<<18;
}
void LCD_data(unsigned char data)
{
   IOCLR0=0xFF<<16;
   IOSET1=1<<16;
   IOCLR1=1<<17;
   IOSET0=data<<16;
   IOSET1=1<<18;
   ms_delay(10);
   IOCLR1=1<<18;
}
 
void UART1_init(void)
{
	U1LCR =0x9F;
	U1DLL =0x61;
	U1DLM =0x00;
	U1LCR = 0x1F;
}
unsigned char UART1_Rxbyte(void)	    //rxbyte -- recive byte
{   char val;
    while(!(U0LSR & 0X01));
	{
		val = U0RBR;
		return val;
		
	}
}
void LCD_Write_string(unsigned char *str)
{
	int i=0;
	while(str[i]!='\0')
	{
		LCD_data(str[i]);
		ms_delay(100);
		i++;
	}
}