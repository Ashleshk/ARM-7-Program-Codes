#include  <lpc214x.h>							  //header files
#include <string.h>

#define PCLK			15000000            	   //Clock settings
#define BAUDRATE	9600	 						//Set Baud Rate here

void  delay_ms(unsigned int time)	   
{  
 unsigned int  i, j;
 for (j=0; j<time; j++)
  for(i=0; i<8002; i++);
}

void LCD_command(unsigned char command)
{
	IOCLR0 = 0xFF<<16;							// Clear LCD Data lines
	IOCLR1=1<<16;		 						// RS=0 for command
	IOCLR1=1<<17;		 						// RW=0 for write
	IOSET0=command<<16;							// put command on data line
	IOSET1=(1<<18);								// en=1 
	delay_ms(10) ;								// delay
	IOCLR1=(1<<18);	 							// en=0
}

LCD_data(unsigned char data)
{
	IOCLR0 = 0xFF<<16;							// Clear LCD Data lines
	IOSET1=1<<16;		 						// RS=1 for data
	IOCLR1=1<<17;		 						// RW=0 for write
	IOSET0= data<<16; 							// put command on data line
	IOSET1=(1<<18);								//en=1 
	delay_ms(10) ;	  							//delay
	IOCLR1=(1<<18);								//en=0
 }

LCD_init()
{
	LCD_command(0x38);				
	delay_ms(10) ;		
	LCD_command(0x0c);				//display on, cursor off, cursor char blinking off(display on/off)
	delay_ms(10) ;			
	LCD_command(0x06);  			//cursor increament and display shift(entry mode set)
	delay_ms(10) ;			
	LCD_command(0x01); 				//clear lcd(clear command)
	delay_ms(10) ;			
	LCD_command(0x80); 				//set cursor to 0th location	1st lne
	
}
 
LCD_write_string(unsigned char *string)
{
  while(*string)				//Check for End of String
  LCD_data(*string++); 			//sending data on LCD byte by byte
}

void  Init_UART1(void)					//This function setups UART0
{  
   unsigned int Baud16;
   U1LCR = 0x83;		            // DLAB = 1
   Baud16 = (PCLK /(16*BAUDRATE));  
   U1DLM = Baud16 >>8;							
   U1DLL = Baud16 ;						
   U1LCR = 0x03;
}

void  UART1_SendByte(unsigned char data)	//send a byte on UART0
{  
   U1THR = data;				    
   while( (U1LSR&0x40)==0 );	    
}

unsigned char  UART1_RecievedByte( )	   //receive byte on UART0
{     
  while((U1LSR&0x01)==0);
   	return U1RBR; 
	    
}
   
int  main(void)
{  
	unsigned char string1[] = "GPGGA";
	unsigned char i, flag=0;
	unsigned char cmd_data[5], time_data[15], latitude[15], longitude[15],garbage; 
	PINSEL0 = 0x00050005; 
	IODIR1= 0x07<<16;	
	IODIR0=	0xFF<<16; 
	LCD_init();				
	Init_UART1();
	
	LCD_write_string("AP LAB");
	LCD_command(0xc0);//second line
	LCD_write_string("GPS_Interface");
	delay_ms(1000);
	
	
	while(1)
	{
	if(UART1_RecievedByte()=='$')
		{
			for (i=0; i<5; i++)
			{	
			flag=0;
			cmd_data[i]=UART1_RecievedByte();
			if (cmd_data[i]!= string1[i])
				{
					flag = 1;
					break;
				}
			}
		 if (flag == 0)
		 {	 garbage=UART1_RecievedByte();
			for (i=0; i<11; i++)
			{	
				time_data[i]=UART1_RecievedByte();
			} 
			time_data[11]='\0';
			
		 
			for (i=0; i<12; i++)
			{	
				latitude[i]=UART1_RecievedByte();
			}
			latitude[11]='\0';
			
			for (i=0; i<12; i++)
			{	
				longitude[i]=UART1_RecievedByte();
			}
			longitude[12]='\0';
			
			LCD_command(0x01);
			LCD_write_string("GPS sent:");
			LCD_write_string(cmd_data);
			LCD_command(0xC0);
			LCD_write_string("UTC:");
			LCD_write_string(time_data);
			delay_ms(2000);
			LCD_command(0x01);
			LCD_write_string("Ln:");
			LCD_write_string(longitude);
			LCD_command(0xC0);
			LCD_write_string("Lt:");
			LCD_write_string(latitude);
			delay_ms(1000);
		 }
		}
	}			
 
}

   




 
 
 






   