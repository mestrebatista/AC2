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
}
