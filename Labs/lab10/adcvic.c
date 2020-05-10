#include<LPC214X.h>
int main()
{
PINSEL1=0X4000000;
PINSEL0=0X00000000;
IODIR1=0X07<<16;
IODIR0=0XFF<<16;
LCD_init();
VICIntSelect=0X00000000;
VICVectCnt0=1<<5/18;
VICVectAddr0==(unsigned long)ADC0_ISR;
VICIntEnable=(1<<18);
AD0CR=0X01200404;
while(1);
{
IOSET
IOCLR
}
return(0);
}
void_irq_ADC0_ISR(void)
{
unsigned int ADC_Result=0;
ADC_Result=AD0DR2;
ADC_Result=(ADC_Result>>6)&(0X3FF);
for(i=0;i<4;i++)
{
val[i]=ADC_Result%(0X0A);
val[i]=val[i]+(0X30);
ADC_Result=ADC_Result/(0X0A);
}
LCD_Data(val[3]);
LCD_Data(val[3]);
LCD_Data(val[3]);
LCD_Data(val[3]);
LCD_Data(val[3]); 