#include <lpc214x.h> 

void  delay_ms(unsigned char time)	   
{  
 unsigned int  i, j;
 for (j=0; j<time; j++)
  for(i=0; i<8002; i++);
}

void LCD_command(unsigned char command)
{
	IOCLR0 = 0xFF<<16;	 
	IOCLR1=1<<16;		 	 
	IOCLR1=1<<17;		 	 
	IOSET0=command<<16;	 
	IOSET1=(1<<18);		 
	delay_ms(10) ;		 
	IOCLR1=(1<<18);	 	 
}

LCD_data(unsigned char data)
{
	IOCLR0 = 0xFF<<16;	 
	IOSET1=1<<16;		 	 
	IOCLR1=1<<17;		 	 
	IOSET0= data<<16; 	 
	IOSET1=(1<<18);			 
	delay_ms(10) ;	  	 
	IOCLR1=(1<<18);			 
 }

LCD_init()
{
	LCD_command(0x38);	 
	delay_ms(10) ;		 
	LCD_command(0x80); 	 
	delay_ms(10) ;		 
	LCD_command(0x01); 	 
	delay_ms(10) ;			 
	LCD_command(0x06);   
	delay_ms(10) ;		 
	LCD_command(0x0c);	  
}
 
LCD_write_string(unsigned char *string)
{
  while(*string)			//Check for End of String
  LCD_data(*string++); 	//sending data on LCD byte by byte
}
unsigned char val[];
void __irq ADC0_ISR(void)
{
	unsigned int ADC_Result=0;
	unsigned char i;
	ADC_Result = AD0DR2;  
	ADC_Result = (ADC_Result>>6) & 0x3FF;
	LCD_command (0xCA);
	for(i=0;i<4;i++)
	{
	val[i]=ADC_Result%0x0A;
	val[i]=val[i]+0x30;
	ADC_Result=ADC_Result/0x0A;

	}
	LCD_data(val[3]);
	LCD_data(val[2]);
	LCD_data(val[1]);
	LCD_data(val[0]);
	delay_ms(10);
	VICVectAddr=0x00;		 

	 
}
int main(void)
{

	PINSEL1 = 0x04000000; 
	PINSEL2 = 0X00;	 
	IODIR1= 0x07<<16;  
	IODIR0=	0xFF<<16;  
	LCD_init();
	LCD_write_string("ADC CHANNEL (itr)");
	LCD_command(0xc0);//second line
	LCD_write_string("AD0.2 O/P=");
	VICIntSelect=0x00000000;
	VICVectCntl0=(1<<5)|18;
	VICVectAddr0=(unsigned long)ADC0_ISR;
	VICIntEnable=1<<18;
	AD0CR = 0x01200404;	
	while(1) ;
		 
	

	 
}