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
