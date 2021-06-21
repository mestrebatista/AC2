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
