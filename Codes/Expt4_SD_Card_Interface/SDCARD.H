#ifndef __SDCARD_H__
#define __SDCARD_H__

#define MMC_DATA_SIZE 								512 				/* 512 bytes */
#define MAX_TIMEOUT 									0xFF
#define IDLE_STATE_TIMEOUT 						1
#define OP_COND_TIMEOUT 							2
#define SET_BLOCKLEN_TIMEOUT 					3
#define WRITE_BLOCK_TIMEOUT 					4
#define WRITE_BLOCK_FAIL 							5
#define READ_BLOCK_TIMEOUT 						6
#define READ_BLOCK_DATA_TOKEN_MISSING	7
#define DATA_TOKEN_TIMEOUT 						8
#define SELECT_CARD_TIMEOUT 					9
#define SET_RELATIVE_ADDR_TIMEOUT 		10


unsigned char CMD0[6] = {0x40,0x00,0x00,0x00,0x00,0x95};	//for initialization
unsigned char CMD1[6] = {0x41,0x00,0x00,0x00,0x00,0xFF};	//to check if card is ready
unsigned char CMD16[6] = {0x50,0x00,0x00,0x02,0x00,0xFF}; //Set Block Length
unsigned char CMD24[6] = {0x58,0x00,0x00,0x02,0x00,0xFF}; //Write Block
unsigned char CMD17[6] = {0x51,0x00,0x00,0x02,0x00,0xFF}; //read block

unsigned int sdcard_init(void);
unsigned int sdcard_read_block(unsigned int block_number, unsigned char *MMCRDData);
unsigned int sdcard_write_block(unsigned int block_number, unsigned char *MMCWRData);

unsigned int sdcard_wait_for_write_finish(void);
unsigned int sdcard_response(unsigned char response);
/************************** SDCARD Init *********************************/

/* Initialises the SDCARD into SPI mode and sets block size(512), returns  0 on success */

unsigned int sdcard_init()
{
 unsigned int i;
 unsigned char sdcard_status = 0;
 IOSET0 = SPI_SEL; 		/* set SPI SSEL */
 
 for(i=0; i<10; i++)	/* initialise the MMC card into SPI mode by sending 80 clks and keep MOSI line high */
 {
  SPI_SendByte (0xFF);	 /* Use MMCRDData as a temporary buffer for SPI_Send() */
 }

 IOCLR0 = SPI_SEL; 		 /* clear SPI SSEL */
 /* send CMD0(RESET or GO_IDLE_STATE) command, all the arguments are 0x00 for the reset command, precalculated checksum */

 SPI_Send( CMD0, 6 );
 if( sdcard_response(0x01) == 1 )	/* if = 1 then there was a timeout waiting for 0x01 from the MMC */
 {
  sdcard_status = IDLE_STATE_TIMEOUT;
  IOSET0 = SPI_SEL; /* set SPI SSEL */
  return sdcard_status;
 }
 
 IOSET0 = SPI_SEL; /* set SPI SSEL */	   /* Send some dummy clocks after GO_IDLE_STATE */
 SPI_ReceiveByte();
 IOCLR0 = SPI_SEL; /* clear SPI SSEL */
 
 i = MAX_TIMEOUT;			   /* must keep sending command until zero response ia back. */
 do
 {
	/* send mmc CMD1(SEND_OP_COND) to bring out of idle state */ /* all the arguments are 0x00 for command one */
   SPI_Send( CMD1, 6 );
  i--;
 } while ( (sdcard_response(0x00) != 0) && (i>0) );
 
 if ( i == 0 )			/* timeout waiting for 0x00 from the MMC */
 {
  sdcard_status = OP_COND_TIMEOUT;
  IOSET0 = SPI_SEL; /* set SPI SSEL */
  return sdcard_status;
 }
 
 IOSET0 = SPI_SEL; /* set SPI SSEL */		  /* Send some dummy clocks after SEND_OP_COND */
 SPI_ReceiveByte();
 IOCLR0 = SPI_SEL; /* clear SPI SSEL */
 
// Send MMC CMD16(SET_BLOCKLEN) to set the block length */
 SPI_Send( CMD16, 6 );
 if( (sdcard_response(0x00))==1 )
 {
  sdcard_status = SET_BLOCKLEN_TIMEOUT;
  IOSET0 = SPI_SEL; /* set SPI SSEL */
  return sdcard_status;
 }
 IOSET0 = SPI_SEL; /* set SPI SSEL */
 SPI_ReceiveByte();
 return 0;
}


/************************** SDCARD Write Block ***************************/
/* write a block of data based on the length that has been set
* in the SET_BLOCKLEN command.
* Send the WRITE_SINGLE_BLOCK command out first, check the
* R1 response, then send the data start token(bit 0 to 0) followed by
* the block of data. The test program sets the block length to 512
* bytes. When the data write finishs, the response should come back
* as 0xX5 bit 3 to 0 as 0101B, then another non-zero value indicating
* that MMC card is in idle state again.
*
*/
unsigned int sdcard_write_block(unsigned int block_number, unsigned char *MMCWRData)
{
 unsigned int var;
 unsigned char Status, sdcard_status=0;
 IOCLR0 = SPI_SEL; /* clear SPI SSEL */

 var=(block_number)<<9;		/* Aligning block address with linear address */
  
 //MMCCmd[0] = 0x58;						/* send mmc CMD24(WRITE_SINGLE_BLOCK) to write the data to MMC card */
 CMD24[1] = (var)>>24;				/* high block address bits, varh HIGH and LOW */
 CMD24[2] = (var)>>16;
 CMD24[3] = (var)>>8;				/* low block address bits, varl HIGH and LOW */
 CMD24[4] = var;
 //MMCCmd[5] = 0xFF;
 						/* checksum is no longer required but we always send 0xFF */
 SPI_Send(CMD24,6);
 if((sdcard_response(0x00))==1)			/* if mmc_response returns 1 then we failed to get a 0x00 response */
 {
  sdcard_status = WRITE_BLOCK_TIMEOUT;
  IOSET0 = SPI_SEL; /* set SPI SSEL */
  return sdcard_status;
 } 
 /* Set bit 0 to 0 which indicates the beginning of the data block */
 SPI_SendByte (0XFE);
 SPI_Send( MMCWRData, MMC_DATA_SIZE );  /*send data*/
 /* Send dummy checksum */
 /* when the last check sum is sent, the response should come back
 immediately. So, check the SPI FIFO MISO and make sure the status
 return 0xX5, the bit 3 through 0 should be 0x05 */
 
 SPI_SendByte (0XFF);
 SPI_SendByte (0XFF);

 Status = SPI_ReceiveByte();
 if ( (Status & 0x0F) != 0x05 )
 {
  sdcard_status = WRITE_BLOCK_FAIL;
  IOSET0 = SPI_SEL; /* set SPI SSEL */
  return sdcard_status;
 } 
 /* if the status is already zero, the write hasn't finished
 yet and card is busy */
 if(sdcard_wait_for_write_finish()==1)
 {
  sdcard_status = WRITE_BLOCK_FAIL;
  IOSET0 = SPI_SEL; /* set SPI SSEL */
  return sdcard_status;
 } 
 IOSET0 = SPI_SEL; /* set SPI SSEL */
 SPI_ReceiveByte();
 return 0;
}


/************************** SDCARD Read Block ****************************/
/*
* Reads a 512 Byte block from the MMC
* Send READ_SINGLE_BLOCK command first, wait for response come back
* 0x00 followed by 0xFE. The call SPI_Receive() to read the data
* block back followed by the checksum.
*
*/
unsigned int sdcard_read_block(unsigned int block_number, unsigned char *MMCRDData)
{
 unsigned int Checksum;
 unsigned int var;
 unsigned char sdcard_status=0;
 IOCLR0 = SPI_SEL; /* clear SPI SSEL */
 var=(block_number)<<9;
 
/* send MMC CMD17(READ_SINGLE_BLOCK) to read the data from MMC card */
 CMD17[1] = (var)>>24;				/* high block address bits, varh HIGH and LOW */
 CMD17[2] = (var)>>6;
 CMD17[3] = (var)>>8;				/* low block address bits, varl HIGH and LOW */
 CMD17[4] = var;
 CMD17[5] = 0xFF;						/* checksum is no longer required but we always send 0xFF */
 SPI_Send(CMD17,6);
 
 if((sdcard_response(0x00))==1)			/* if mmc_response returns 1 then we failed to get a 0x00 response */
 {
  sdcard_status = READ_BLOCK_TIMEOUT;
  IOSET0 = SPI_SEL; /* set SPI SSEL */
  return sdcard_status;
 }

 if((sdcard_response(0xFE))==1)			/* wait for data token */
 {
 sdcard_status = READ_BLOCK_DATA_TOKEN_MISSING;
 IOSET0 = SPI_SEL;
 return sdcard_status;
 }
 
 SPI_Receive( MMCRDData, MMC_DATA_SIZE );/* Get the block of data based on the length */
 /* CRC bytes that are not needed */
 Checksum = SPI_ReceiveByte();
 Checksum = Checksum << 0x08 | SPI_ReceiveByte();
 IOSET0 = SPI_SEL; /* set SPI SSEL */
 SPI_ReceiveByte();
 return 0;
}


/***************** SDCARD get response *******************/
/*
* Repeatedly reads the MMC until we get the
* response we want or timeout
*/
unsigned int sdcard_response(unsigned char response)
{
 unsigned int count = 0xFFF;
 unsigned char result;
 while( count > 0 )
 {
  result = SPI_ReceiveByte();
  if ( result == response )
  {
   break;
  }
  count--;
 }
 if ( count == 0 )
 return 1; /* Failure, loop was exited due to timeout */
 else
 return 0; /* Normal, loop was exited before timeout */
}

/***************** SDCARD wait for write finish *******************/
/*
* Repeatedly reads the MMC until we get a non-zero value (after
* a zero value) indicating the write has finished and card is no
* longer busy.
*
*/
unsigned int sdcard_wait_for_write_finish( void )
{
 unsigned int count = 0xFFFF; /* The delay is set to maximum considering
 the longest data block length to handle */
 unsigned char result = 0;
 while( (result == 0) && count )
 {
 result = SPI_ReceiveByte();
 count--;
 }
 if ( count == 0 )
 return 1; /* Failure, loop was exited due to timeout */
 else
 return 0; /* Normal, loop was exited before timeout */
}
#endif