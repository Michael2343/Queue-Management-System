#include <AT89C5131.h>

sbit Seg1= P1^0;
sbit Seg2= P1^1;
sbit Seg3= P1^2;

sbit D0= P1^3;
sbit D1= P1^4;
sbit D2= P1^5;
sbit D3= P1^6;

sbit buzzer= P1^7;
sbit Station1 = P2^3;

int num1=0,num2=0;
int NumStation=0;
int n=1;


void Delay(int msec)   //delay
{
	int i,j; 
	for(j=0;j<msec;j++) 
	for(i=0;i<200;i++);
}
void number(int i)
{
D0=i&0x01;
  i=i >> 1;

D1=i&0x01;
  i=i >> 1;

D2=i&0x01;
  i=i >> 1;

D3=i&0x01; 
}
void digit(int i)
{
 if (i==1)
	{
  Seg1= 1;
  Seg2= 0;
  Seg3= 0;
	}
	else if (i==2)
	{
  Seg1= 0;
  Seg2= 1;
  Seg3= 0;
	}
	else if (i==3)
	{
  Seg1= 0;
  Seg2= 0;
  Seg3= 1;
	}
	
}
void setup_Seg()
{
  Seg1= 0;
  Seg2= 0;
  Seg3= 0;
  buzzer=0;	
  IT1=1; 

TMOD=0x01;		// timer 0 mode 1															
TL0=-2000&0xff;   // timer 0  -  1 msec interrupt
TH0=-2000>>8;
TR0=1;
IEN0=0x86;  // enable timer 0 , int	1	
}
   void Timer0() interrupt 1   // timer 0
{

TR0=0;
TL0=-2000&0xff;   // timer 0  -  1 msec interrupt
TH0=-2000>>8;
TR0=1;

switch (n) 
{
    case 1:
	digit(1);
	number(num1);  
	break;

    case 2:
	digit(2);
	number(num2);	  
		break;

    case 3:
	digit(3);
	number(NumStation);	  
		break;


}
n++;   // next segment
if (n==4) n=1;
}

  void station() interrupt 2	//int 1
{
 
  
  if(Station1==0x00) NumStation=2;
  else if(Station1==0x01)  NumStation=1;	
  Delay(200);

  num2++;

  if(num2>9)
  {
	num2=0;
	num1++;
	if(num1>9) num1=0;
  }	

   buzzer=1;
   Delay(100);
   buzzer=0;
   TCON=TCON&0xF7;
  
}		 
void main()
{
setup_Seg();

while(1)
{
   buzzer=1;
   Delay(500);
   buzzer=0;
   Delay(500);
/*	digit(1);
	number(num1);  
	Delay(5);

    digit(2);
	number(num2);	  
	Delay(5);

	digit(3);
	number(line);	  
	Delay(5);  */
}	
}

