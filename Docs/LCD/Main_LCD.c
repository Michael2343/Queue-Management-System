 #include <AT89C5131.h>
sbit RS = P2^0;
sbit LCD_EN = P2^1;
sbit keypad_EN=P2^2;

char FirstName[]="Michael\0";
char LastName[]="Ohayon\0";

void Delay(int msec)   //delay
{
	int i,j; 
	for(j=0;j<msec;j++) 
	for(i=0;i<200;i++);
}
 

void LCD_Write(char command,char cnt) //write display
{
	if(cnt == 'c') RS=0; 
  else if(cnt == 'd')  RS=1;	                      //For Command (=c) or data (=d)
	
LCD_EN=1;
Delay(5);
P0=command;
LCD_EN=0;
Delay(10);
}

void ClrScr()                 //clear Screen
{
	 LCD_Write(0x01,'c');
}
void CursorFirst()            //Cursor go to (0,0) first place
{ 
	 LCD_Write(0x02,'c');
}

void NewLine()                //new line Cursor go to line 2
{
	 LCD_Write(0xc0,'c');
}

void SendWord(char Letters[])  //send the String
{
	int i=0;
	while(Letters[i]!='\0') 
	{
	LCD_Write(Letters[i],'d');
	i++;
	}
}

void setup()  //Display Options
{
	LCD_Write(0x06,'c'); //cursor movement
	LCD_Write(0x0c,'c'); //Appearance
  LCD_Write(0x38,'c'); //Cursor display
}

void main()
{
	keypad_EN = 1;
	setup();
	ClrScr();
	CursorFirst();
	SendWord(FirstName);
	Delay(3000);
	ClrScr();
	CursorFirst();
	SendWord(FirstName);
	newline();
	SendWord(LastName);
	Delay(3000);
	ClrScr();
	CursorFirst();
	
}
