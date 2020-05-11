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
void  delay_ms(unsigned char time)	   //This Function is used to cause delay between LED ON and OFF events
{  
 unsigned int  i, j;
 for (j=0; j<time; j++)
  for(i=0; i<8002; i++);
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
	
	PINSEL0 = 0x00050005; // Enable UART0 Rx and Tx pins
  Init_UART0();
  
UART0_SendStr("AT+CMGF=1\r");	
	delay_ms(250);
		delay_ms(250);
		delay_ms(250);
		delay_ms(250);
		delay_ms(250);
		delay_ms(250);
  UART0_SendStr("AT+CMGS=\"+919637331857\"\r");
	delay_ms(250);
		delay_ms(250);
		delay_ms(250);
		delay_ms(250);
	delay_ms(250);
	UART0_SendStr("GSM Interface");
		delay_ms(250);
	delay_ms(250);
	delay_ms(250);
	delay_ms(250);
	delay_ms(250);
	UART0_SendByte(26);
  
	
	while(1);	
   
}

   




 
 
 






   
