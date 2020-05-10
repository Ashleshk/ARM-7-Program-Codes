/**************************************************************************************************
Expt. 5a.: Interface I2C EEPROM to LPC2148
Platform: Smart Logic LPC2148 Development Board.
		
Clock Settings:
	FOSC	>>	12MHz (onboard)
	PLL		>>	M=5, P=2
	CCLK	>>  60MHz
	PCLK	>>  15MHz 
		
Hardware Setup:-
	LCD data pin :-P0.16-P0.23 
	RS-P1.16
	RW-P1.17
	EN-P1.18	        
********************************************************************************/

//Include Controller specific header file
#include <lpc214x.h> 

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
	LCD_command(0x80); 	//set cursor to 0th location	1st lne
	delay_ms(10) ;			// delay
	LCD_command(0x01); 	//clear lcd(clear command)
	delay_ms(10) ;			// delay
	LCD_command(0x06);  //cursor increament and display shift(entry mode set)
	delay_ms(10) ;			// delay
	LCD_command(0x0c);	//display on, cursor off, cursor char blinking off(display on/off)
}
 
LCD_write_string(unsigned char *string)
{
  while(*string)			//Check for End of String
  LCD_data(*string++); 	//sending data on LCD byte by byte
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

unsigned char I2C_Status(unsigned char status_code)
{
	unsigned char status, SI_Flag;
	while(!((SI_Flag=I2C0CONSET)& 0x08));
	status=I2C0STAT;				//Read Status byte
	I2C0CONCLR=0x28;				//Clear SI bit
	if(status!=status_code)
	{
		return 1;
	}
	else
	{
  return 0;
	}
}

unsigned char byte_write(unsigned char device,unsigned char address,unsigned char data)
{
	I2C0CONCLR=0xFF;	//Clear All bits
	I2C0CONSET=0x40;	//Set I2C Enable
	Send_Start();
	if(I2C_Status(0x08))	//Start has been transmitted
	{ return 1;	}

	I2C0DAT= device&0xFE;          //Sending Device Address
	if(I2C_Status(0x20))					//Device address, block num and write has been transmitted
	{  return 1; }

	I2C0DAT= address;				 //Sending Memory Location
	if(I2C_Status(0x30))	//Block address has been transmitted
	{	return 1; }

	I2C0DAT= data;				 //Sending data
	if(I2C_Status(0x30))	//Block address has been transmitted
	{	return 1; }
  delay_ms(10);
	Send_Stop();
	delay_ms(10);
	return 0;
}


unsigned char byte_read(unsigned char device,unsigned char address)
{
	unsigned char data;
	I2C0CONCLR=0xFF;	//Clear All bits
	I2C0CONSET=0x40;	//Set I2C Enable	
	Send_Start();
	if(I2C_Status(0x08))	//Start has been transmitted
	{  return 1; }

	I2C0DAT= device&0xFE;  //Selecting device in write mode
	if(I2C_Status(0x20))	//Device address, block num and write has been transmitted
	{  return 1; }

	I2C0DAT= address;		//Sending Memory Location
	if(I2C_Status(0x30))	//Block address has been transmitted
	{  return 1; }

	Send_Start();		     // Repeat Start
	if(I2C_Status(0x10))	//Repeated Start has been transmitted
	{  return 1; }

	I2C0DAT= device | 0x01;			//Device address, block num and read has been transmitted
	if(I2C_Status(0x48))	//
	{  return 1; }
 
	I2C0CONCLR=0x04;		 //set hardware to send nack
	if(I2C_Status(0x58))	//last byte has been received and NACK has been returned
	{    return 1;   }
	data=I2C0DAT; 
 
	Send_Stop();
	return data;
}



int main(void)
{
	unsigned char read_data;
	PINSEL0 = 0x00000055;
	PINSEL1 = 0x00;		//Configure PORT0 as GPIO
	PINSEL2 = 0X00;		//Configure PORT1 as GPIO
	IODIR1= 0x07<<16;	//Configure P1.18, P1.17, P1.16 as output
	IODIR0=	0xFF<<16; //Configure P0.23 - P0.16 as output
	LCD_init();				//Initialize LCD 16x2
	I2C_Init();
	LCD_write_string("Smart Logic Tech");
	LCD_command(0xc0);//second line
	LCD_write_string("Embed. Processors");
	
	byte_write (0xA0,0x20,0x41);
	delay_ms (100);
	read_data = byte_read (0xA0,0x20);
	LCD_command (0x01);
	LCD_data (read_data);
	while (1);
	
}