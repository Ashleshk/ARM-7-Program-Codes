/**************************************************************************************************
Expt. 2a: UART Communication			
Platform: Smart Logic LPC2148 Development Board.

Hardware Setup:-
	Connect a DB9 cable between PC and UART0 or UART1.
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
	unsigned char rx_data[20], i;
	PINSEL0 = 0x00050005; // Enable UART0 Rx and Tx pins
  Init_UART1();
  
	UART1_SendStr("Welcome to E&TC Department, PICT\n\r ");	
  UART1_SendStr("LPC2148 DEVLOPMENT BOARD\n\r ");
	UART1_SendStr("Send 10 Characters\n\r ");
  
	for (i=0; i<10; i++)
	{	
 		rx_data[i]=UART1_RecievedByte();
  }   
	rx_data [10]= 0; // To make received string as ASCIIZ string
	UART1_SendStr("\n\rTransmitted Characters are:");
	UART1_SendStr_ram (rx_data);
	UART1_SendStr("\n\rRx Tx test complete\n\r");
	while(1);	
   
}

   




 
 
 






   
