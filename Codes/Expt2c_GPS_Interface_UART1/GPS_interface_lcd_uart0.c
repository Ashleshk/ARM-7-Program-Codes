/**************************************************************************************************
Expt. 2C: GPS Module Interface			
Platform: Smart Logic LPC2148 Development Board.

Hardware Setup:-
	Connect a DB9 cable between GSM Module and UART0.
	COMPORT Settings
	Baudrate:-9600
	Databits:-8
	Parity:-None
	Stopbits:1
		

Clock Settings:
	FOSC	>>	12MHz (onboard)
	PLL		>>	M=5, P=2
	CCLK	>>  60MHz
	PCLK	>>  15MHz 
********************************************************************************/
//Include controller specific header file
#include  <lpc214x.h>	


//Define Clock settings
#define PCLK			15000000            
#define BAUDRATE	9600	 //Set Baud Rate here


//Function to generate software delay
//Calibrated to 1ms
void  delay_ms(unsigned int time)	   
{  
 unsigned int  i, j;
 for (j=0; j<time; j++)
  for(i=0; i<8002; i++);
}

void LCD_command(unsigned char command)
{
	IOCLR0 = 0xFF<<16;	// Clear LCD Data lines
	IOCLR1=1<<16;		 		// RS=0 for command
	IOCLR1=1<<17;		 		// RW=0 for write
	IOSET0=command<<16;	// put command on data line
	IOSET1=(1<<18);			// en=1 
	delay_ms(10) ;			// delay
	IOCLR1=(1<<18);	 		// en=0
}

LCD_data(unsigned char data)
{
	IOCLR0 = 0xFF<<16;	// Clear LCD Data lines
	IOSET1=1<<16;		 		// RS=1 for data
	IOCLR1=1<<17;		 		// RW=0 for write
	IOSET0= data<<16; 	// put command on data line
	IOSET1=(1<<18);			//en=1 
	delay_ms(10) ;	  	//delay
	IOCLR1=(1<<18);			//en=0
 }

LCD_init()
{
	LCD_command(0x38);	//8bit mode and 5x8 dotes (function set)
	delay_ms(10) ;			// delay
	LCD_command(0x0c);	//display on, cursor off, cursor char blinking off(display on/off)
	delay_ms(10) ;			// delay
	LCD_command(0x06);  //cursor increament and display shift(entry mode set)
	delay_ms(10) ;			// delay
	LCD_command(0x01); 	//clear lcd(clear command)
	delay_ms(10) ;			// delay
	LCD_command(0x80); 	//set cursor to 0th location	1st lne
	
}
 
LCD_write_string(unsigned char *string)
{
  while(*string)			//Check for End of String
  LCD_data(*string++); 	//sending data on LCD byte by byte
}


//Function to Initialize the UART0
void  Init_UART0(void)					//This function setups UART0
{  
   unsigned int Baud16;
   U0LCR = 0x83;		            // DLAB = 1
   Baud16 = (PCLK /(16*BAUDRATE));  
   U0DLM = Baud16 >>8;							
   U0DLL = Baud16 ;						
   U0LCR = 0x03;
}
				
//Functin to Transmit the characters
void  UART0_SendByte(unsigned char data)	   //A function to send a byte on UART0
{  
   U0THR = data;				    
   while( (U0LSR&0x40)==0 );	    
}

//Functin to Receive the characters
unsigned char  UART0_RecievedByte( )	   //A function to send a byte on UART0
{     
  while((U0LSR&0x01)==0);
   	return U0RBR; 
	    
}

//Functin to Transmit the string
void  UART0_SendStr(const unsigned char *str)	 //A function to send a string on UART0
{  
   while(1)
   {  
      if( *str == '\0' ) break;
      UART0_SendByte(*str++);	    
   }
}

void  UART0_SendStr_ram(unsigned char *str)	 //A function to send a string on UART0
{  
   while(1)
   {  
      if( *str == '\0' ) break;
      UART0_SendByte(*str++);	    
   }
}
 


int  main(void)
{  
	unsigned char string1[] = "GPGGA";
	unsigned char i, flag=0;
	unsigned char cmd_data[5], time_data[15], latitude[15], longitude[15]; 
	PINSEL0 = 0x00000005; // Enable UART0 Rx and Tx pins
	IODIR1= 0x07<<16;	//Configure P1.18, P1.17, P1.16 as output
	IODIR0=	0xFF<<16; //Configure P0.23 - P0.16 as output
	LCD_init();				//Initialize LCD 16x2
	Init_UART0();
	
	LCD_write_string("Smart Logic Tech");
	LCD_command(0xc0);//second line
	LCD_write_string("GPS_Interface");
	delay_ms(1000);
	
	
	while(1)
	{
	if(UART0_RecievedByte()=='$')				//Start of GPS Sentence
		{
			for (i=0; i<5; i++)
			{	
			flag=0;
			cmd_data[i]=UART0_RecievedByte();	//Receive GPS sentence type
			if (cmd_data[i]!= string1[i])		//Verify GPS sentence with GPGGA
				{
					flag = 1;
					break;
				}
			}
		 if (flag == 0)
		 {
			for (i=0; i<12; i++)			
			{	
				time_data[i]=UART0_RecievedByte(); //Receive the Time 
			} 
			time_data[12]='\0';
			
			for (i=0; i<12; i++)			
			{	
				latitude[i]=UART0_RecievedByte(); //Receive the Latitude
			}
			latitude[11]='\0';
			
			for (i=0; i<12; i++)
			{	
				longitude[i]=UART0_RecievedByte(); //Receive the Longitude
			}
			longitude[12]='\0';
			
			LCD_command(0x01);
			LCD_write_string("GPS sent:");		
			LCD_write_string(cmd_data);			//Display command
			LCD_command(0xC0);
			LCD_write_string("UTC:");
			LCD_write_string(time_data);			//Display time
			delay_ms(2000);
			LCD_command(0x01);
			LCD_write_string("Ln:");
			LCD_write_string(longitude);			//Display Longitude
			LCD_command(0xC0);
			LCD_write_string("Lt:");
			LCD_write_string(latitude);			//Display Latitude
			delay_ms(1000);
		 }
		}
	}			
		
	
	
   

}

   




 
 
 






   
