#include <AT89C5131.h>

unsigned char press;
sbit LCD_EN = P2^1;
sbit keypad_EN=P2^2;

char key_code[]={0xD,0x00,0x1,0x2,0x4,0x5,0x6,0x8,0x9,0xA,0x3,0x7,0xB,0xF,0xE};
unsigned char key_table[]="0123456789ABCD#";

void Delay(int msec)   //delay
{
	int i,j; 
	for(j=0;j<msec;j++) 
	for(i=0;i<200;i++);
}

char ASCII_TO_KEY(unsigned char key)
{
	int i;
	for(i=0;i<15;i++)
	{
		if(key_code[i]==key)
			return(key_table[i]);
	}
	return '*';
}
	


void main()
{
	
	char key;
	LCD_EN=1; 
	IEN0=0x81;	
	IT0=1;   
	press=0; 
	keypad_EN=0; 
  P1=0x00;	
	
while(1)
{	
  while(press==0);  
	press=0;   
	Delay(5); 
	key=P0;   
	key=key&0x0f; 
	key=ASCII_TO_KEY(key);  
	P1=key;
	Delay(100);
}
}

void pressTrue() interrupt 0
{
 press=1;
}