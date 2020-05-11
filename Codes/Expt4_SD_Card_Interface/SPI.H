#ifndef __SPI_H__
#define __SPI_H__

#define SPI_SEL  0x00000080	/* SPI select pin */

void SPI_Init( void );
void SPI_Send( unsigned char *Buf, unsigned int Length );
unsigned char SPI_SendByte(unsigned char data);
void SPI_Receive( unsigned char *Buf, unsigned int Length );
unsigned char SPI_ReceiveByte( void );

/* SPI and MMC commands related modules. */
void SPI_Init( void )
{
 unsigned int portConfig;
 unsigned char i, Dummy;
 
 portConfig = PINSEL0;			  /* Configure PIN connect block */
 PINSEL0 = portConfig | 0x1500;   /* P0.4, P0.5, P0.6, P0.7 are set as SCK, MISO, MOSI and GPIO */
 
 IODIR0 = SPI_SEL; 				  /* SSEL is output */
 IOSET0 = SPI_SEL; 				  /* set SSEL to high */
 S0SPCCR=8;						  /* SPI clock prescale register minimum value is 8. */
 S0SPCR=0x0030;					  /* Device select as master, Set data to 8-bit, CPOL = 0, CPHA = 0*/ 
 for ( i = 0; i < 8; i++ )		  
 {
 Dummy = S0SPDR; 				  /* clear the RxFIFO */
 }
 return;
}


 
 /* SPI Send a block of data based on the length */
 
void SPI_Send( unsigned char *buf, unsigned Length )
{
 unsigned char Dummy;
 if ( Length == 0 )
 return;
 while ( Length != 0 )
 {
  S0SPDR = *buf;
  while ( !(S0SPSR & 0x80) );	/* Wait until the SPIF bit is set to indicate trabsfer complete */
  Dummy = S0SPDR; 				/* Flush the RxFIFO */
  Length--;
  buf++;
 }
 return;
}


/* SPI Send a single byte  */
unsigned char SPI_SendByte(unsigned char data)
{
	S0SPDR = data;   									//write to SPI data register
	while(!(S0SPSR & 0x80));					   	//wait for SPIF flag to set (data transmitted)
	data = S0SPDR;								   	//read SPI data register
	return data;
} 


/* SPI receives a block of data based on the length */
void SPI_Receive( unsigned char *buf, unsigned int Length )
{
 unsigned int i;
 for ( i = 0; i < Length; i++ )
 {
  *buf = SPI_ReceiveByte();
  buf++;
 }
 return;
}




/* SPI Receive Byte, receive one byte only, return Data byte
   used a lot to check the status.   */
unsigned char SPI_ReceiveByte( void )
{
 unsigned char data;
 S0SPDR = 0xFF;					/* wrtie dummy byte out to generate clock, then read data from  MISO */
 while (!(S0SPSR & 0x80) );		/* Wait until the SPIF bit is set to indicate trabsfer complete */
 data = S0SPDR;
 return data;
}

#endif 