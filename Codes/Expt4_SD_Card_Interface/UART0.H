#ifndef __UART0_H__
#define __UART0_H__


/**************************************************************************************************
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

//Define Clock settings
#define PCLK			15000000            
#define BAUDRATE	9600	 //Set Baud Rate here

void  Init_UART0(void);
void  UART0_SendByte(unsigned char data);
unsigned char  UART0_RecievedByte(void);
void  UART0_SendStr(const unsigned char *str);
void  UART0_SendStr_ram(unsigned char *str);

//Function to Initialize the UART0
void  Init_UART0(void)					//This function setups UART0
{  
  unsigned int Baud16;
	PINSEL0 = PINSEL0 & 0xFFFFFFF0;
	PINSEL0 = PINSEL0 | 0x00000005;	
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
unsigned char  UART0_RecievedByte(void)	   //A function to send a byte on UART0
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


#endif