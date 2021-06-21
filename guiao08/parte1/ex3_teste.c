#include <detpic32.h>
void delay(unsigned int ms){
for(;ms>0;ms--){
 resetCoreTimer();
 while(readCoreTimer()<20000);
 }
}
void putc(char byte2send)    {
while(U2STAbits.UTXBF ==1);// wait while UTXBF == 1
U2TXREG = byte2send;// Copy byte2send to the UxTXREG register
}
void putstr(char *str)    {
 while(*str != '\0'){// use putc() function to send each charater ('\0' should not

 //  be sent)
 putc(*str);
 str++;
 }
}
int main(void)    {
// Configure UART2:
unsigned int baudrate = 115200;
unsigned int fpbclk = 20000000;
U2BRG = ((fpbclk + 8*baudrate)/(16*baudrate))-1; // 1 - Configure BaudRate Generator
U2MODEbits.PDSEL = 0;// 2 – Configure number of data bits, parity and number of stop bits
U2MODEbits.STSEL = 1;//     (see U1MODE register)
U2STAbits.UTXEN = 1;
U2STAbits.URXEN = 1;// 3 – Enable the trasmitter and receiver modules (see register U1STA)
U2MODEbits.ON = 1; // 4 – Enable UART2 (see register U1MODE)
while(1)       {
 putstr("String de teste\n");

 delay(1000);// wait 1 s
 }
}
