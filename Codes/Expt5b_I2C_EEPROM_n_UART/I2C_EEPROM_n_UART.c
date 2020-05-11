/**************************************************************************************************
Expt. 5b.: Interface I2C EEPROM to LPC2148
Display the contents on UART
Platform: Smart Logic LPC2148 Development Board.
		
Clock Settings:
	FOSC	>>	12MHz (onboard)
	PLL		>>	M=5, P=2
	CCLK	>>  60MHz
	PCLK	>>  15MHz 
		
Hardware Setup:-
	Connect a DB9 cable between PC and UART0 or UART1.
	COMPORT Settings
	Baudrate:-9600
	Databits:-8
	Parity:-None
	Stopbits:1     
********************************************************************************/

//Include Controller specific header file
#include <lpc214x.h> 

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



void I2C_Init()
{
 
 I2C0CONCLR=0xFF;	//Clear All bits
 I2C0CONSET=0x40;	//Set I2C Enable
 I2C0SCLH=75;			//Set I2C_Clock = 100Khz
 I2C0SCLL=75;			//with 50% duty cycle
}

void Send_Start()
{
 I2C0CONSET=0x20; 
}


void Send_Stop()
{
 I2C0CONSET=0x10;
}


unsigned char byte_write(unsigned char device,unsigned char address,unsigned char data)
{
	
	I2C0CONCLR=0xFF;	//Clear All bits
	I2C0CONSET=0x40;	//Set I2C Enable	
	
	Send_Start();			//Send Start Condition
	while(!(I2C0CONSET& 0x08)); //Check SI Flag
	I2C0CONCLR=0x28;	//Clear SI flag
	
	I2C0DAT= device&0xFE; //Selecting device in write mode
	while(!(I2C0CONSET& 0x08)); //Check SI Flag
	I2C0CONCLR=0x28;	//Clear SI flag
	
	I2C0DAT= address;  //Sending Memory Location
	while(!(I2C0CONSET& 0x08)); //Check SI Flag
	I2C0CONCLR=0x28;	//Clear SI flag
	
	I2C0DAT= data;				 //Sending data
	while(!(I2C0CONSET& 0x08)); //Check SI Flag
  I2C0CONCLR=0x28;	
	
	Send_Stop();		//send stop condition
	
	return 0;
}


unsigned char byte_read(unsigned char device,unsigned char address)
{
	unsigned char data;

	I2C0CONCLR=0xFF;	//Clear All bits
	I2C0CONSET=0x40;	//Set I2C Enable	
	
	Send_Start();			//Send Start Condition
	while(!(I2C0CONSET& 0x08)); //Check SI Flag
	I2C0CONCLR=0x28;	//Clear SI flag
	
	I2C0DAT= device&0xFE; //Selecting device in write mode
	while(!(I2C0CONSET& 0x08)); //Check SI Flag
	I2C0CONCLR=0x28;	//Clear SI flag
	
	I2C0DAT= address;		//Sending Memory Location
	while(!(I2C0CONSET& 0x08)); //Check SI Flag
	I2C0CONCLR=0x28;	//Clear SI flag
	
	Send_Start();		   // Repeat Start
	while(!(I2C0CONSET& 0x08)); //Check SI Flag
	I2C0CONCLR=0x28;	//Clear SI flag
	
	I2C0DAT= device | 0x01;	//Selecting device in read mode
	while(!(I2C0CONSET& 0x08)); //Check SI Flag
	I2C0CONCLR=0x28;	//Clear SI flag
	I2C0CONCLR=0x04;		 //set hardware to send nack
	
	while(!(I2C0CONSET& 0x08)); //Check SI Flag
	I2C0CONCLR=0x28;	//Clear SI flag
	data=I2C0DAT;	
	
	Send_Stop();		//send stop condition
	return data;
}



int main(void)
{
	unsigned char read_data, i;
	unsigned char write_data[]= "EEPROM I2C TEST ";
	PINSEL0 = 0x00000055; //Configure Port pin as I2C & UART pin
	PINSEL1 = 0x00;		//Configure PORT0 as GPIO
	PINSEL2 = 0X00;		//Configure PORT1 as GPIO
	IODIR1= 0x07<<16;	//Configure P1.18, P1.17, P1.16 as output
	IODIR0=	0xFF<<16; //Configure P0.23 - P0.16 as output
	
	I2C_Init();
	Init_UART0();
  
	UART0_SendStr("Smart Logic Technologies\n\r ");	
  UART0_SendStr("Embedded Processors\n\r ");
	UART0_SendStr("Writing to I2C: ");
	
	for (i=0;i<16; i++)
	{	
	byte_write (0xA0,i, write_data[i]);
	UART0_SendByte(write_data[i]);	
	delay_ms (50);
	}
	delay_ms (500);
	UART0_SendStr("\n\rReading from I2C: ");
	for (i=0;i<16; i++)
	{	
	read_data = byte_read (0xA0,i);
	UART0_SendByte(read_data);		
	delay_ms (50);
	}
	
	while (1);
	
}