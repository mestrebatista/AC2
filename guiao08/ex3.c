#include <detpic32.h>

void configUart1(unsigned int baud, char parity, unsigned int stopbits)
{
  if (baud<600 || baud>115200) {
    baud=11520;
    U1BRG = ((PBCLK+16/2*baud)/(16*baud))-1;
    U1MODEbits.BRGH=0;
  }
  else{// Configure BaudRate Generator
  U1BRG = ((PBCLK+16/2*baud)/(16*baud))-1;
  U1MODEbits.BRGH=0;
  }
  // Configure number of data bits (8), parity and number of stop bits
  if (parity=='N' || parity=='n')
  {
    U1MODEbits.PDSEL=00; //8 databits e sem paridade
  }
  if(parity=='E' || parity=='e')
  {
    U1MODEbits.PDSEL=01; //8 databits e paridade par
  }
  if(parity=='O' || parity=='o')
  {
    U1MODEbits.PDSEL=10; //8 databits e paridade impar
  }
  else
  {
    U1MODEbits.PDSEL=00; //9 databits e sem paridade
  }

  if(stopbits>2)
  {
    U1MODEbits.STSEL=0;
  }
  else
  {
    U1MODEbits.STSEL=stopbits-1;
  }
  // Enable the trasmitter and receiver modules
  U1STAbits.UTXEN=1;
  U1STAbits.URXEN=1;
  // Enable UART2
  U1MODEbits.ON=1;

}

void delay(int ms)
{
  for (;ms>0; ms--)
  {
    resetCoreTimer();
    while(readCoreTimer()<20000);
  }
}

void putc(char byte2send)
{
  if (U1STAbits.OERR==1)
  {
    U1STAbits.OERR=0;
  }

  while(U1STAbits.UTXBF==1);// wait while UTXBF == 1
  U1TXREG=byte2send;// Copy byte2send to the UxTXREG register
}

void puts(char *str)
{
  // use putc() function to send each charater ('\0' should not
  // be sent)
  while (*str!='\0') {
    putc(*str);
    str++;
  }
}

int main(void)
{

  configUart1(115200,'N',1);

  while(1)
  {
    puts("String de teste\n");
    delay(1000);// wait 1 s
  }
  return 0;
}
