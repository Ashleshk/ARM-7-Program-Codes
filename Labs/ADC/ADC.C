#include <lpc214x.h> 
//Calibrated to 1ms
void  delay_ms(unsigned int time)    
{  
 unsigned int  i, j;
 for (j=0; j<time; j++)
  for(i=0; i<8002; i++);
}

void LCD_command(unsigned char command)
{
 IOCLR0 = 0xFF<<16; // Clear LCD Data lines
 IOCLR1=1<<16;     // RS=0 for command
 IOCLR1=1<<17;     // RW=0 for write
 IOSET0=command<<16; // put command on data line
 IOSET1=(1<<18);   // en=1 
 delay_ms(10) ;   // delay
 IOCLR1=(1<<18);    // en=0
}

LCD_DATA(unsigned char data)
{
 IOCLR0 = 0xFF<<16; // Clear LCD Data lines
 IOSET1=1<<16;     // RS=1 for data
 IOCLR1=1<<17;     // RW=0 for write
 IOSET0= data<<16;  // put command on data line
 IOSET1=(1<<18);   //en=1 
 delay_ms(10) ;    //delay
 IOCLR1=(1<<18);   //en=0
 }

LCD_init()
{
 LCD_command(0x38); //8bit mode and 5x8 dotes 
 delay_ms(10) ;   // delay
 LCD_command(0x01);  //clear lcd(clear command)
 delay_ms(10) ;   // delay
 LCD_command(0x06);  //cursor increament and 
 delay_ms(10) ;   // delay
 LCD_command(0x0E);
 delay_ms(10) ;
 LCD_command(0x80);  //set cursor to 0th location 1st lne
 delay_ms(10) ; 
}

void LCD_write_string(unsigned char *string)
{
  int i = 0;
  while(string[i]!='\0')  //Check for End of String
  {
  LCD_DATA(string[i]);
  delay_ms(10) ;
  i=i++;  //sending data on LCD byte by byte
  }
} 

int main(void)
{
  unsigned char var1[]= {"ADC O/P :"};
 unsigned char val[4];
 unsigned int ADC_Result=0,i;
 PINSEL1 = 0x04000000; 
 PINSEL2 = 0X00;  //Configure PORT1 as GPIO
 IODIR1= 0x07<<16; //Configure P1.18, P1.17, P1.16 
 IODIR0= 0xFF<<16; //Configure P0.23 - P0.16 as output
 //IODIR0= 0X0F<<8;
 LCD_init();    //Initialize LCD 16x2
  /************** Configure ADC0 for following
 ADC Channel = AD0.2
 ADC Clock = 3 MHz
 Clock Selection = 11 Clock Cycles/10bit
 Start Condition = No start
 Power Down = 1, EDGE = 0, BURST = 0 *************/
 AD0CR = 0x01200404;
 
 while (1)
 {
     LCD_write_string(var1);
     AD0CR |= 1<<24; //Start ADC 
  while (!(AD0DR2&0x80000000));  //Wait for  conversion
   ADC_Result = AD0DR2; //Store converted data
  ADC_Result = (ADC_Result>>6) & 0x3FF;
   
   for(i=0;i<4;i++)
 {
  val[i]=ADC_Result%0X0A ;
  val[i]=val[i]+0X30;
   ADC_Result=ADC_Result/0X0A;
 }
   
    LCD_DATA(val[3]);
    LCD_DATA(val[2]);
    LCD_DATA(val[1]);
    LCD_DATA(val[0]);
    delay_ms(1200);
    LCD_command(0x01);  //clear lcd(clear command)
  
   
 }
}