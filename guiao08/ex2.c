#include <detpic32.h>

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


int main(void)
{
    // Configure UART2: 115200 bps, sem paridade, 8 data bits, 1 stop bit
    // 1 - Configure BaudRate Generator
  int baudrate=115200;
  U1BRG = ((PBCLK+8*baudrate)/(16*baudrate))-1;
  U1MODEbits.BRGH=0;
    // 2 – Configure number of data bits, parity and number of stop bits
    //     (see U1MODE register)
  U1MODEbits.PDSEL=0; //8 databits e sem paridade
  U1MODEbits.STSEL=0;//1 stop bit
    // 3 – Enable the trasmitter and receiver modules (see register U1STA)
  U1STAbits.UTXEN=1;
  U1STAbits.URXEN=1;
    // 4 – Enable UART2 (see register U1MODE)
  U1MODEbits.ON=1;
  while(1)
  {
    putc('+');
    delay(1000);// wait 1 s
  }
  return 0;
}
