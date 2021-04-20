#include <detpic32.h>

void delay(int ms)
{
  for (;ms>0; ms--)
  {
    resetCoreTimer();
    while (readCoreTimer()<200000);
  }
}

int main(void)
{
  unsigned char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F,\
                      0x66, 0x6D, 0x7D, 0x07,\
                      0x7F, 0x6F, 0x77, 0x7C,\
                      0x39, 0x5E, 0x79, 0x71};
  unsigned char counter = 0;
  LATDbits.LATD6 = 1;                 // display high active
  LATDbits.LATD5 = 0;                 // display low inactive
  TRISB= TRISB & 0x80FF;              // configure RB8-RB14 as outputs
  TRISD= TRISD & 0xFF9F;              // configure RD5-RD6 as outputs

  while(1)
  {
    LATDbits.LATD6 = !LATDbits.LATD6; //
    LATDbits.LATD5 = !LATDbits.LATD5; // toggle display selection

    LATB=(LATB & 0x80FF) | (display7Scodes[counter]<<8); // send "segment" value to display
    delay(500);                     // wait 0.5 second
    counter++;
    if(counter==16)
    {
      counter=0;
    }
  }
  return 0;
}
