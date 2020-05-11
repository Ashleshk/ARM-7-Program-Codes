/**************************************************************************************************
Expt. 2b: GSM Module Interface for sendind SMS			
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
void  delay_ms(unsigned char time)	   
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
void  Init_UART1(void)					//This function setups UART0
{  
   unsigned int Baud16;
   U1LCR = 0x83;		            // DLAB = 1
   Baud16 = (PCLK /(16*BAUDRATE));  
   U1DLM = Baud16 >>8;							
   U1DLL = Baud16 ;						
   U1LCR = 0x03;
}
				
//Functin to Transmit the characters
void  UART1_SendByte(unsigned char data)	   //A function to send a byte on UART0
{  
   U1THR = data;				    
   while( (U1LSR&0x40)==0 );	    
}

//Functin to Receive the characters
unsigned char  UART1_RecievedByte( )	   //A function to send a byte on UART0
{     
  while((U1LSR&0x01)==0);
   	return U1RBR; 
	    
}

//Functin to Transmit the string
void  UART1_SendStr(const unsigned char *str)	 //A function to send a string on UART0
{  
   while(1)
   {  
      if( *str == '\0' ) break;
      UART1_SendByte(*str++);	    
   }
}

void  UART1_SendStr_ram(unsigned char *str)	 //A function to send a string on UART0
{  
   while(1)
   {  
      if( *str == '\0' ) break;
      UART1_SendByte(*str++);	    
   }
}
 


int  main(void)
{  
	unsigned char i;
	unsigned char rx_data[]= "GSM test data  ";; 
	PINSEL0 = 0x00050005; // Enable UART0 Rx and Tx pins
  
	IODIR1= 0x07<<16;	//Configure P1.18, P1.17, P1.16 as output
	IODIR0=	0xFF<<16; //Configure P0.23 - P0.16 as output
	LCD_init();				//Initialize LCD 16x2
	Init_UART1();
	
	LCD_write_string("Smart Logic Tech");
	LCD_command(0xc0);//second line
	LCD_write_string("GSM_Interface");
	delay_ms(1000);
	LCD_command(0x01);
	LCD_write_string("AT CMD-Mfg ID");
	
	UART1_SendStr("AT+CGMI\n\r");	
	for (i=0; i<15; i++)
	{	
 		rx_data[i]=UART1_RecievedByte();
  } 
	rx_data [15]= 0; // To make received string as ASCIIZ string
	delay_ms(1000);
	LCD_command(0xc0);//second line
	LCD_write_string (rx_data);
	while(1);	
   
}

   




 
 
 






   
