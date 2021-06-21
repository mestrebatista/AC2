#include <detpic32.h>

#define DisableUart1RxInterrupt() IEC0bits.U2RXIE = 0
#define EnableUart1RxInterrupt()  IEC0bits.U2RXIE = 1
#define DisableUart1TxInterrupt() IEC0bits.U2TXIE = 0
#define EnableUart1TxInterrupt()  IEC0bits.U2TXIE = 1


typedef struct
{
  unsigned char data[BUF_SIZE];
  unsigned int head;
  unsigned int tail;
  unsigned int count;
} circularBuffer;

volatile circularBuffer txb;  //Transmission Buffer
volatile circularBuffer rxb;  //Reception Buffer

#define BUF_SIZE    8
#define INDEX_MASK  (BUF_SIZE-1)

void comDrv_flushRx(void)
{
  // Initialize variables of the reception buffer
  rxb.head=0;
  rxb.tail=0;
  rxb.count=0;
}

void comDrv_flushTx(void)
{
  // Initialize variables of the transmission buffer
  txb.head=0;
  txb.tail=0;
  txb.count=0;
}

void comDrv_putc(char ch)
{
  while(txb.count == BUF_SIZE){}          // Wait while buffer is full
  txb.data[txb.tail] = ch;                // Copy character to the transmission
                                          // buffer at position "tail"
  txb.tail = (txb.tail + 1) & INDEX_MASK; // Increment "tail" index
                                          // (mod. BUF_SIZE)
  DisableUart1TxInterrupt()               // Begin of critical section
  txb.count++;                            // Increment "count" variable
  EnableUart1TxInterrupt();               // End of critical section
}

void comDrv_puts(char *s)
{
  while (*s!='\0')
  {
    comDrv_putc(*s);
    s++;
  }
}

void _int_(24) isr_uart2(void)
{
  if(IFSObits.U2TXIF==1)// if U1TXIF is set
  {
    if(txb.count>0)// if "count" variable (transmission buffer, txb) is greater than 0
    {
      U2TXREG=txb.data[txb.head];// Copy character pointed by "head" to U1TXREG register
      txb.head = (txb.head + 1) & INDEX_MASK;// Increment "head" variable (mod BUF_SIZE)
      txb.count--;// Decrement "count" variable
    }
    if(txb.count==0)// if "count" variable is 0 then
    {
      DisableUart1TxInterrupt();
    }
    IFSObits.U2TXIF=0;// Reset UART1 TX interrupt flag
  }

  if(IFSObits.U2TXIF==1)// If U1RXIF is set
  {
    rxb.data[rxb.tail] = U1RXREG; // Read character from UART and
                                  // write it to the "tail" position
                                  // of the reception buffer
    rxb.tail=(rxb.tail+1) & INDEX_MASK; //Increment "tail" variable (mod BUF_SIZE)
    //If reception buffer is not full (e.g. count < BUF_SIZE) then
    //increment "count" variable
    if (rxb.count<BUF_SIZE) {
      rxb.count++;
    }
    //Else
    //increment "head" variable (discard oldest character)
    else
    {
      rxb.head=(rxb.head +1) & INDEX_MASK;
    }
    //reset UART1 RX interrupt flag
    IFSObits.U2TXIF=0;
  }
}

void comDrv_config(unsigned int baud, char parity, unsigned int stopbits)
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
void configInterrupts()
{
  IPC6bits.U1IP = 2;		// configure priority of interrupts
	IFS0bits.U1RXIF = 0;	// clear interrupt flag (RX)
	IFS0bits.U1TXIF = 0;	// clear interrupt flag (TX)
	IEC0bits.U1RXIE = 1;	// enable interrupts (RX)
	IEC0bits.U1TXIE = 1;	// enable interrupts (TX)
}

char comDrv_getc(char *pchar)
{
  // Test "count" variable (reception buffer) and return FALSE
  // if it is zero
  if(rxb.count==0) return FALSE;
  DisableUart1RxInterrupt();          // Begin of critical section
  *pchar=rxb.data[rxb.head];          // Copy character pointed by "head" to *pchar
  rxbcount--;                         // Decrement "count" variable
  rxb.head=(rxb.head+1) & INDEX_MASK; // Increment "head" variable (mod BUF_SIZE)
  EnableUart1RxInterrupt();           //End of critical section
  return TRUE;
}

int main(void)
{
  comDrv_config(115200,'N',1); // default "pterm" parameters
  configInterrupts();// with TX and RX interrupts disabled

  comDrv_flushRx();
  comDrv_flushTx();
  EnableInterrupts();
  while(1)
    comDrv_puts("Teste do bloco de transmissao do device driver!...");

  comDrv_config(115200,'N',1); // default "pterm" parameters
  configInterrupts();// with TX and RX interrupts disabled

  comDrv_flushRx();
  comDrv_flushTx();
  EnableInterrupts();
  comDrv_puts("PIC32 UART Device-driver\n");

  char aux;
  while(1)
  {
		if(comDrv_getc(&aux))
    {
      // Send character to the transmission buffer
			comDrv_putc(aux);
    }
}
