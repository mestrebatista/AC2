#include <detpic32.h>

int main(void)
{
    // Configure UART2: 115200 bps, sem paridade, 8 data bits, 1 stop bit
    // 1 - Configure BaudRate Generator
  int baudrate=115200;
  U2BRG = ((PBCLK+8*baudrate)/(16*baudrate))-1;
  U2MODEbits.BRGH=0;
    // 2 – Configure number of data bits, parity and number of stop bits
    //     (see U1MODE register)
  U2MODEbits.PDSEL=00; //8 databits e sem paridade
  U2MODEbits.STSEL=0;//1 stop bit
    // 3 – Enable the trasmitter and receiver modules (see register U1STA)
  U2STAbits.UTXEN=1;
  U2STAbits.URXEN=1;
    // 4 – Enable UART2 (see register U1MODE)
  U2MODEbits.ON=1;

  return 0;
}
