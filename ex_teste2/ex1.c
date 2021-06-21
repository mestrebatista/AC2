#include <detpic32.h>
void delay(int ms)
{
  for(;ms>0; ms--)
  {
    resetCoreTimer();
    while (readCoreTimer()<20000);
  }
}

int main(void) {

  LATE=(LATE & 0xFFF0);

  TRISE=(TRISE & 0xFFF0);
  TRISB=(TRISB & 0x000F);

  while (1)
  {
    LATEbits.LATE0=PORTBbits.RB0;
    LATEbits.LATE1=PORTBbits.RB1;
    LATEbits.LATE2=PORTBbits.RB2;
    LATEbits.LATE3=PORTBbits.RB3;
  }
  return 0;
}
