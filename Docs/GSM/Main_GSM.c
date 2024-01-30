#include <AT89C5131.h>
char Phone_num[10]="0547853886";
void delay(int milliseconds) //Function that delays the Microprocessor
{
	int i,j; //local variables used for counting
	for(j = 0;j<milliseconds;j++) //number of milliseconds
	for(i = 0;i<200;i++); //200 achieved through measurements of the output periods
}

void serialprint(char a[]) //Function that sends strings through UART
{
  int i=0; //indexer
	while ( a[i]!='\0') //runs until reached end of string
	{
	SBUF = a[i]; //load char a[i] to buffer
	while(TI==0); //wait for sent approval 
		TI=0; //revert uart tx
		i++; //increment indexer

	}
}
void newline_GSM() // Function that sends Carriage return and Newline
{
		SBUF = 0x0D;
	  while(TI==0); //0x0A -> NEWLINE, 0x0D - CARRIAGE RETURN 
		TI=0;
		SBUF = 0x0A;
	  while(TI==0); //0x0A -> NEWLINE, 0x0D - CARRIAGE RETURN 
		TI=0;
	
}

void main() //MAIN
{

CKCON0+=0x01; //increase BDR
SCON=0x50;  // UART MODE 1 (8BIT REGISTER), ENABLE RECIEVE
PCON+=0x80; //Increase BDR
BDRCON+=0x1E; //Incrase BDR, Run INT_BDG, Set Int_BRG to be the UARTS Timer
BRL=177;    //SET BAUD_RATE TO 19200
	
delay(2000); //DELAY 2SEC
serialprint("AT+CMGF=1"); //print COMMAND
newline_GSM(); //send newline
delay(1500); //DELAY 1.5SEC
serialprint("AT+CMGS=\"");
serialprint(Phone_num);
serialprint("\""); //print COMMAND   
newlinee_GSM(); //send newline
RI=0; //prepare for recieve

 
	while(RI==0); //freeze until data recieved
	RI=0; //revert recieve
	if(SBUF == '>') //check if recieved approval
	 {
	 P2_0=0; //turn on p2_0
	 P1=SBUF; //load recieved data onto p1
	
	 delay(500); //delay 0.5sec
 	 serialprint("Hello Michael"); //send message		
	 delay(500); //DELAY 0.5SEC
		
	 SBUF = 0x1A; //subtitute (END MESSAGE)
	 while(TI==0); 
	 TI=0;
		
   newline(); //newline
	 delay(5000); //DELAY 5SEC
   
 }
}
