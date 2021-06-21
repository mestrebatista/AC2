#include <detpic32.h>

void configUart2(unsigned int baud, char parity, unsigned int stopbits)
{
  if (baud<600 || baud>115200) {
    baud=11520;
    U2BRG = ((PBCLK+16/2*baud)/(16*baud))-1;
    U2MODEbits.BRGH=0;
  }
  else{// Configure BaudRate Generator
  U2BRG = ((PBCLK+16/2*baud)/(16*baud))-1;
  U2MODEbits.BRGH=0;
  }
  // Configure number of data bits (8), parity and number of stop bits
  if (parity=='N')
  {
    U2MODEbits.PDSEL=00; //8 databits e sem paridade
  }
  if(parity=='E')
  {
    U2MODEbits.PDSEL=01; //8 databits e paridade par
  }
  if(parity=='O')
  {
    U2MODEbits.PDSEL=10; //8 databits e paridade impar
  }
  else
  {
    U2MODEbits.PDSEL=00; //9 databits e sem paridade
  }

  if(stopbits>2)
  {
    U2MODEbits.STSEL=0;
  }
  else
  {
    U2MODEbits.STSEL=stopbits-1;
  }
  // Enable the trasmitter and receiver modules
  U2STAbits.UTXEN=1;
  U2STAbits.URXEN=1;
  // Enable UART2
  U2MODEbits.ON=1;

}

void delay(int ms)
{
  for (;ms>0; ms--)
  {
    resetCoreTimer();
    while(readCoreTimer()<20000);
  }
}

void put1char(char byte2send)
{
  if (U2STAbits.OERR==1)
  {
    U2STAbits.OERR=0;
  }

  while(U2STAbits.UTXBF==1);// wait while UTXBF == 1
  U2TXREG=byte2send;// Copy byte2send to the UxTXREG register
}

void putstring(char *str)
{
  // use putc() function to send each charater ('\0' should not
  // be sent)
  while (*str!='\0') {
    put1char(*str);
    str++;
  }
}

int main(void)
{
  configUart2(115200,'N',1);
  TRISBbits.TRISB6=0;// config RB6 as output
  
  while(1)
  {
    while(U2STAbits.TRMT==0);// Wait until TRMT == 1
    LATBbits.LATB6=1;// Set RB6
    putstring("12345\n");
    LATBbits.LATB6=0;// Reset RB6
  }
}
