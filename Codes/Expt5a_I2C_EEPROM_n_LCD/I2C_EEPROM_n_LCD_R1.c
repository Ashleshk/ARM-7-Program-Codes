
#include <lpc214x.h> 

void  delay_ms(unsigned int time)	   
{  
 unsigned int  i, j;
 for (j=0; j<time; j++)
  for(i=0; i<8002; i++);
}

void LCD_command(unsigned char command)
{
	IOCLR0 = 0xFF<<16;	   
	IOCLR1=1<<16;		 	
	IOCLR1=1<<17;		 	
	IOSET0=command<<16;	
	IOSET1=(1<<18);			
	delay_ms(10) ;		
	IOCLR1=(1<<18);	 	
}

LCD_data(unsigned char data)
{
	IOCLR0 = 0xFF<<16;	
	IOSET1=1<<16;		 	
	IOCLR1=1<<17;		 	
	IOSET0= data<<16; 
	IOSET1=(1<<18);		
	delay_ms(10) ;	  	
	IOCLR1=(1<<18);		
 }

LCD_init()
{
	LCD_command(0x38);	
	delay_ms(10) ;		
	LCD_command(0x80); 	
	delay_ms(10) ;			
	LCD_command(0x01); 	
	delay_ms(10) ;		
	LCD_command(0x06); 
	delay_ms(10) ;			
	LCD_command(0x0c);	
}
 
LCD_write_string(unsigned char *string)
{
  while(*string)			
  LCD_data(*string++); 	
}


void I2C_Init()
{
 
 I2C0CONCLR=0xFF;		//Clear All bits
 I2C0CONSET=0x40;		//Set I2C Enable
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
	while(!(I2C0CONSET&0x08)); //Check SI Flag
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
	//to switch the mode use repeat strat condition
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
	unsigned char write_data[4]= "PICT";
	PINSEL0 = 0x00000050; //Configure Port pin as I2C pin
	PINSEL1 = 0x00;		//Configure PORT0 as GPIO
	PINSEL2 = 0X00;		//Configure PORT1 as GPIO
	IODIR1= 0x07<<16;	//Configure P1.18, P1.17, P1.16 as output
	IODIR0=	0xFF<<16; //Configure P0.23 - P0.16 as output
	LCD_init();				//Initialize LCD 16x2
	I2C_Init();
	LCD_write_string("Embed. Processors");
	LCD_command(0xc0); 
	LCD_write_string("Practicals");
	delay_ms (500);
	LCD_command (0x01);
	LCD_write_string("Writing to I2C");
	LCD_command (0xC0);
	for (i=0;i<4; i++)
	{	
	byte_write (0xA0,i, write_data[i]);
	LCD_data (write_data[i]);
	delay_ms (50);
	}
	delay_ms (500);
	LCD_command (0x01);
	LCD_write_string("Reading from I2C");
	LCD_command (0xC0);
	for (i=0;i<4; i++)
	{	
	read_data = byte_read (0xA0,i);
	LCD_data (read_data);	
	delay_ms (50);
	}
	
	while (1);
	
}
