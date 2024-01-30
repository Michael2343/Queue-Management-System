#include <AT89C5131.h>
#include <stdio.h>
#include <string.h>

sbit LCD_RS = P2^0;
sbit LCD_EN = P2^1;
sbit keypad_EN=P2^2;
char Phone_num[10];
char key_code[]={0xD,0x00,0x1,0x2,0x4,0x5,0x6,0x8,0x9,0xA,0x3,0x7,0xB,0xF,0xE};
unsigned char key_table[]="0123456789ABCD#";

bit SendSMS=0;
unsigned char k=0;
unsigned char cnt=0;
char key;

sbit Seg1= P1^0;
sbit Seg2= P1^1;
sbit Seg3= P1^2;

sbit D0= P1^3;
sbit D1= P1^4;
sbit D2= P1^5;
sbit D3= P1^6;

sbit Buzzer= P1^7;
sbit Station1 = P2^3;
sbit LedStation1 = P2^4;
sbit LedStation2 = P2^5;

unsigned char num1=0,num2=0;//7 seg disply number of line (2 digits)
unsigned char numcnt=0;//7 seg number counter
unsigned char NumStation=0;//7 seg disply number free station
unsigned char n=1;//7 seg disply counter jumper
unsigned char Line[2]={'0','1'};
unsigned char linecnt=0;//line number counter
  
unsigned long time=0;
unsigned int time1=0;
unsigned int amount=1;
unsigned int avg=20;//Avg per queue
unsigned int avgt=0;//Avg per user

unsigned char avgasc[3]={'0','2','0'};
unsigned char avgtotal[3]={'0','2','0'};


void Delay(int msec)//delay
{
	int i,j; 
	for(j=0;j<msec;j++) 
	for(i=0;i<200;i++);
}


void LCD_Write(char command,char cnt) //write display
{
if(cnt == 'c') LCD_RS=0;      // What we want to disply on LCD
else if(cnt == 'd')  LCD_RS=1;//cnt = For Command (=c) or data (=d)
	
LCD_EN = 1;
Delay(5);
P0=command;
LCD_EN = 0;
Delay(10);
}
  
void ClrScr_CursorFirst()//clear Screen && Cursot to (0,0)
{
	 LCD_Write(0x01,'c');
	 LCD_Write(0x02,'c');
}

void NewLine() //new line Cursor go to line 2
{
	 LCD_Write(0xc0,'c');
}

void SendWord(char Letters[])//send the String
{
	int i=0;
	while(Letters[i]!='\0') 
	{
	LCD_Write(Letters[i],'d');
	i++;
	}
}

char CODE_TO_ASCII(unsigned char key)
{
	int i;
	for(i=0;i<15;i++)
	{
		if(key_code[i]==key)
			return(key_table[i]);
	}
	return '*';
}


	void keypad_enable(int x)
	{
	keypad_EN=x;
	}
	
	void setup_KP()
	{
	keypad_EN=1;	
	IT0=1;   
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
	else if (i==0)
	{
  Seg1= 0;
  Seg2= 0;
  Seg3= 0;
  }	
}
void setup_Seg()
{
 digit(0);
 Buzzer=0;	
 IT1=1; 

TMOD=0x01;// timer 0 mode 1															
TL0=-2000&0xff;// timer 0  -  1 msec interrupt
TH0=-2000>>8;
TR0=1; // enable timer 0 , int	1	
}

void setup_LCD()//Display Options
{
LCD_EN =1;
LCD_Write(0x06,'c');//cursor movement
LCD_Write(0x0c,'c');//Appearance
LCD_Write(0x38,'c');//Cursor display
}

void setup_GSM()
{
CKCON0+=0x01;//increase BDR
SCON=0x50;// UART MODE 1 (8BIT REGISTER), ENABLE RECIEVE
PCON+=0x80;//Increase BDR
BDRCON+=0x1E;//Incrase BDR, Run INT_BDG, Set Int_BRG to be the UARTS Timer
BRL=177;//SET BAUD_RATE TO 19200
}

	
void Avg_Total()
  {
  avgt=(linecnt-numcnt+1)*avg;

  avgtotal[2]=(((avgt%100))%10)+0x30;// unity 
  avgtotal[1]=(((avgt%100))/10)+0x30;// Dozen
  avgtotal[0]=(avgt/100)+0x30;// Hundred
  }

void serialprint(char a[])                  
{
  int i=0;
	while ( a[i]!='\0')                      
	{
	SBUF = a[i];                              
	while(TI==0);                            
		TI=0;                                  
		i++; 
	}
}

void serialprint2(char arr[],int x)                  
{	   
int i;

if(arr[0]=='0')
{
 i=1;
  if(arr[1]=='0') i=2;
 }
else i=0;

for(;i<=x-1;i++)
{
	SBUF = arr[i];                              
	while(TI==0);                             
	TI=0; 	
}                                                                    
}
	   
void newline_GSM()// Function that sends Carriage return and Newline
{
		SBUF = 0x0D;
	  while(TI==0);//0x0A -> NEWLINE, 0x0D - CARRIAGE RETURN 
		TI=0;
		SBUF = 0x0A;
	  while(TI==0);//0x0A -> NEWLINE, 0x0D - CARRIAGE RETURN 
		TI=0;	
}

 void SMS_Send()
 {
int flag = 1;
IEN0=IEN0&0xfa;
Delay(2000);//DELAY 2SEC
serialprint("AT+CMGF=1");//print COMMAND
newline_GSM();//send newline
Delay(1500);//DELAY 1.5SEC
serialprint("AT+CMGS=\"");
serialprint(Phone_num);			   
serialprint("\"");//print COMMAND   
newline_GSM();//send newline
RI=0;//prepare for recieve

while(flag)
 {
	while(RI==0);//freeze until data recieved
	RI=0;//revert recieve
	while(SBUF == '>')//check if recieved approval
	 {                                
	 Delay(500); 		           
 	 serialprint("Your Number is: ");//send message
	 serialprint2(Line,2); 
	 newline_GSM();
	 serialprint("Avg time for a Queue: ");
	serialprint2(avgasc,3); 
	serialprint(" Sec ");
	newline_GSM();
	Avg_Total();
	serialprint("Avg time to wait: ");
	serialprint2(avgtotal,3); 
	serialprint(" Sec ");
	
 	flag = 0;
	 Delay(500);//DELAY 0.5SEC
	 SBUF = 0x1A;//subtitute (END MESSAGE)
	 while(TI==0); 
	 TI=0;
   newline_GSM();//newline
	 Delay(5000);//DELAY 5SEC
   
 }
}	  

IEN0=IEN0|0x85;
}
 void LineForword()
 {
   	Line[1]++;
	linecnt++;
	if(	Line[1]=='9'+1) 
	{
		Line[1]='0';
		Line[0]++;
	
		if(Line[0]=='9'+1)
		 {
		 Line[0]=1;
		   Line[1]=0;
		   linecnt=0;
		 }

	}
 }

 void ReciveNum()
	{
	Phone_num[cnt]=key;
	LCD_Write(Phone_num[cnt],'d');
	cnt++;
	Delay(10);
	k++;
	}

void Delete()
{
    char back;
	cnt--;
	k-=1;
	back=0xc0;
	back += (cnt);
	LCD_Write(back,'c');
	LCD_Write(0x20,'d');//20H = space
	LCD_Write(back,'c');
}

void Reset_PhoneNum()
{
int i;
for(i=0;i<=9;i++) 
Phone_num[i]=0;
}
 
void avg_con2_ascii()
  {
  time1=(time/1000)/1.8;// divide by 1.8 = Standard deviation
  time=0;
  avg=avg*amount;
  avg+=time1;
  amount++;
  avg=avg/amount;

  avgasc[2]=(((avg%100))%10)+0x30;// unity 
  avgasc[1]=(((avg%100))/10)+0x30;// Dozen
  avgasc[0]=(avg/100)+0x30;	//Hundred
  }

void PressTrue() interrupt 0
{
    Delay(10);
	P0=0xff;	
	keypad_enable(0); 
	Delay(5);	
	key=P0;   
	keypad_enable(1);
	key=key&0x0f; 
	key=CODE_TO_ASCII(key);  
	
	if(key != 'B' && key != 'A')  ReciveNum();
	else if (key == 'B') Delete();

	else if (key == 'A' && k==10)
	{
	SendSMS=1;
	k=0;
	cnt=0;
	 } 
	}
	
void Timer0() interrupt 1 // timer 0
{  
TR0=0;
TL0=-2000&0xff; // timer 0  -  1 msec interrupt
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
n++;  // next segment
if (n==4) n=1;
time++;
}


  void station() interrupt 2	//int 1
{
  if( numcnt <= linecnt )
  {
   digit(0);
   Buzzer=1;
  Delay(50);
  if(Station1==0x00) 
  {
  NumStation=2;
  LedStation2 = 0;
  Delay(3000);
  LedStation2 = 1;
  }
  else if(Station1==0x01)
  {  
  NumStation=1;
  LedStation1 = 0;
  Delay(3000);
  LedStation1 = 1;
  }

  num2++;
  numcnt++;
  if(num2>9)
  {
	num2=0;
	num1++;

	if(num1>9) 
	{
	num1=0;
	numcnt=0;
	}
  }
   Buzzer=0;
   avg_con2_ascii();
  } 	 
  TCON=TCON&0xF7; 
}

void Setup()
{
	int i; 
	setup_GSM();
	setup_KP();
	setup_LCD();
	setup_Seg();
	Reset_PhoneNum();
	IEN0=0x87;
    ClrScr_CursorFirst();
    SendWord("Load System");
	Delay(2000);
	for(i=0;i<=2;i++)
	{
	 SendWord(".");
	 Delay(1000);
	}
	ClrScr_CursorFirst();
}

void main()
{
   	Setup();
	
	SendWord("Enter Number:");
	NewLine();
    keypad_enable(1);
    	
	while(1)
	{
 	 if(SendSMS)//SendSMS=1 when enter phone number
 	 {
	 SendSMS=0;
	 ClrScr_CursorFirst();
	 SendWord("Number Recived!!");
	 Delay(1000);
	 ClrScr_CursorFirst();
     Delay(1000); 
	 SendWord("SMS Send To:");
	 NewLine();
	 SendWord(Phone_num);
     SMS_Send();//GSM SEND SMS line

	 Reset_PhoneNum();
	 LineForword();

	 Delay(500);
     ClrScr_CursorFirst();
	 SendWord("Enter Number:");
	 NewLine();
	 }
	}
}