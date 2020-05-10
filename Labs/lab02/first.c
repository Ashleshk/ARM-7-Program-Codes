#include<lpc214x.h>

void mydelay(int k){
int i,j;
	for(i=0;i<k;i++){
		for(j=0;j<255;j++){
		}
	}
}

int main(){
		PINSEL0=0x00;
		IODIR0=0x0F;

		while(1){
		IOSET0=0x00F;
		mydelay(777);
		IOCLR0=0x00F;
		mydelay(777);
		}
 return 0;
}

