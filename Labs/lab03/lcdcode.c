#include<lpc214x.h>					  //header files

void LCD_init();					  //method definations			  
void LCD_command(unsigned char cmd)	 ;
void LCD_data(unsigned char data);
void  LCD_write_string(unsigned char *str);
void ms_delay(int k);	  

int main()
{
	unsigned char var1[]="WELCOME";		 //string 1
	unsigned char var2[]="PICT";		  //string 2
	
	PINSEL1=0x00000000;					  //select p0.16-p0.23
	PINSEL2=0X00000000;					  //p1.16-p1.18
	IODIR0=0x0FF<<16;
	IODIR1=0X07<<16;

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
   LCD_command(0x38);				  //select 5x8 cell in 16x2
   ms_delay(100);
   LCD_command(0x01);				  //clear lcd
   ms_delay(100);
   LCD_command(0x0E);				  //set cursor
   ms_delay(100);
   LCD_command(0x06);				  //Auto increment
   ms_delay(100);
   LCD_command(0x80);				  //set cursor at address location
   ms_delay(100)                      //first cell row 0
   
   
}
void LCD_command(unsigned char cmd)	 
{
   IOCLR0=0xFF<<16;				     //clear content in data lines
   IOCLR1=1<<16;					 //rs=0
   IOCLR1=1<<17;					 //rw=0
   IOSET0=cmd<<16;					  //data=cmd
   IOSET1=1<<18;					  //en=1
   ms_delay(10);
   IOCLR1=1<<18;					  //en=0
}
void LCD_data(unsigned char data)
{
   IOCLR0=0xFF<<16;					  //clr data lines
   IOSET1=1<<16;					  //rs=1
   IOCLR1=1<<17;					   //rw=0
   IOSET0=data<<16;					  //data =DATA
   IOSET1=1<<18;					   //en=1
   ms_delay(10);
   IOCLR1=1<<18;					   //en=0
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
