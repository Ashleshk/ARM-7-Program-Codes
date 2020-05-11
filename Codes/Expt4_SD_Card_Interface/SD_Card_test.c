//Include controller specific header file
#include  <lpc214x.h>	
#include "UART0.H"
#include "SPI.H"
#include "SDCARD.H"

int  main(void)
{  
	
	unsigned char read_data[512];
	unsigned char write_data[512]= {"\n\rSmart Logic Techologies\n\rSD Card write and read\n\r"};
  
	Init_UART0();
	SPI_Init();
  
	UART0_SendStr("Smart Logic Technologies\n\r");	
  UART0_SendStr("SD_Card_Testing\n\r ");
	
  UART0_SendStr("sending commands for card init\n\r ");
	
  if ( sdcard_init()!= 0)
	{
		UART0_SendStr("Init Failed\n\r");
		while ( 1 ); /* Very bad happened */
	}
	
	UART0_SendStr("sending commands for writing\n\r ");
	if ( sdcard_write_block(0, write_data) == 0 )
	{
		UART0_SendStr("Write Complete\n\r");
	}
	else
	{
		UART0_SendStr("Write Failed\n\r");
		while(1);
	}
	
	if(sdcard_read_block(0,read_data)==0)
	{	
		UART0_SendStr(read_data);
  }
	else
	{
		UART0_SendStr("Read Failed\n\r");
		while(1);
	}
	
	UART0_SendStr("SD_Card_Tested Successfully\n\r ");
	while(1);	
   
}
