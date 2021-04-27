#include <detpic32.h>

void delay(int ms) {
  for (; ms > 0; ms--)
  {
    resetCoreTimer();
    while(readCoreTimer()<20000);
  }
}

int main(void)
{
  unsigned int i;
  // Configure the A/D module and port RB4 as analog input
  TRISBbits.TRISB4 = 1;
  AD1PCFGbits.PCFG4= 0;
  AD1CON1bits.SSRC = 7;
  AD1CON1bits.CLRASAM = 1;
  AD1CON3bits.SAMC = 16;
  AD1CON2bits.SMPI = 4-1;  // 4 consecutive conversion
  AD1CHSbits.CH0SA = 4;    //desiered input analog channel
  AD1CON1bits.ON = 1;

  while(1)
  {
    AD1CON1bits.ASAM = 1;             // Start conversion

    while( IFS1bits.AD1IF == 0 );     // Wait while conversion not done (AD1IF == 0)

    int *p = (int *)(&ADC1BUF0);
    for( i = 0; i < 16; i++ )
    {
      printInt( p[i*4], 10 | 4 << 16);
      printf(" ");
    }
    IFS1bits.AD1IF = 0;               // Reset AD1IF
  }
  return 0;
}
