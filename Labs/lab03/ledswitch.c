#include<lpc214x.h>

void msdelay(int k)
{
int i,j;
	for(i=0;i<k;i++)
	{
		for(j=0;j<1008;j++)
		{
		}
	}
}

int main()
{
   int i;
   unsigned int X;
   unsigned int val;
   unsigned int sw0;
   unsigned int sw1;
   	PINSEL2=0x00000000;		   //for p1.16-p1.23
	IODIR1=0x00FF0000;		   //make them 	output
    

   while(1)
   {
    	sw0=IOPIN0&0X8000;		 //p0.15
    	sw1=IOPIN0&0X10000;      //p0.16
    
   		if(sw0==0)
		{
			val=1;	  //to store the instance of switch
			          //to make led chasing continuosly
		}
		if(sw1==0)
		{
			val=2;
		} 
		if (val==1)
		{
			X=1<<15;
   			for(i=0;i<8;i++)
   			{
   				X=X<<1;
   				IOSET1=X;			 //set value to pins high
   				msdelay(1000);
   				IOCLR1=X;
				msdelay(1000);
   			}
		}
		if (val==2)
		{
			X=0X01000000;
   			for(i=0;i<8;i++)
   			{
   				X=X>>1;
    			IOSET1=X;
  			    msdelay(1000);
   				IOCLR1=X;
				msdelay(1000);
			}
		}
  
   	  } 

	  return 0;
}