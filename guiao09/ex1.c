#include <detpic32.h>

#define DisableUart1RxInterrupt() IEC0bits.U2RXIE = 0
#define EnableUart1RxInterrupt()  IEC0bits.U2RXIE = 1
#define DisableUart1TxInterrupt() IEC0bits.U2TXIE = 0
#define EnableUart1TxInterrupt()  IEC0bits.U2TXIE = 1
