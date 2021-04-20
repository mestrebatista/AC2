#include <detpic32.h>

void delay(int ms)
{
  for (; ms > 0; ms--)
  {
    resetCoreTimer();
    while(readCoreTimer()<200000);
  }
}

int main(void)
{
  static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F,\
                                        0x66, 0x6D, 0x7D, 0x07,\
                                        0x7F, 0x6F, 0x77, 0x7C,\
                                        0x39, 0x5E, 0x79, 0x71};
  unsigned char counter;
  // configure RB0 to RB3 as inputs
  // configure RB8 to RB14 and RD5 to RD6 as outputs
  LATB=LATB & 0x80FF;
  // Select display low
  LATDbits.LATD5=1;
  LATDbits.LATD6=0;


  TRISB=(TRISB & 0x80FF) | 0x000F;
  TRISD=TRISD & 0xFF9F;
  while(1)
  {
    counter=PORTB & 0x0F;// read dip-switch
    LATB=(LATB & 0x80FF) | display7Scodes[counter]<<8; // convert to 7 segments code
                                                       // send to display
    delay(100);
  }
  return 0;
}
