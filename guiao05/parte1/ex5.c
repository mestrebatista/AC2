#include <detpic32.h>

void delay(int ms) {
  for (; ms > 0; ms--)
  {
    resetCoreTimer();
    while(readCoreTimer()<20000);
  }
}
void send2displays(unsigned char value)
{
  static const char display7Scodes[]={0x3F, 0x06, 0x5B, 0x4F,\
                                      0x66, 0x6D, 0x7D, 0x07,\
                                      0x7F, 0x6F, 0x77, 0x7C,\
                                      0x39, 0x5E, 0x79, 0x71};

  static char displayFlag=0; //variavel estatica para nao perder o seu valor entre estados
  unsigned char digit_low=value & 0x0F;
  unsigned char digit_high=value <<4;

  //toggle displayFlag
  //if flag é 0 entao envia para o display digit_low
  //if flag é 1 entao envia para o display digit_high
  if (displayFlag==0)
  {
    LATDbits.LATD5=1;
    LATDbits.LATD6=0;
    LATB=(LATB & 0x80FF) | (display7Scodes[digit_low]<<8);
  }
  else
  {
    LATDbits.LATD5=0;
    LATDbits.LATD6=1;
    LATB=(LATB & 0x80FF) | (display7Scodes[digit_high]<<8);
  }
  displayFlag=!displayFlag;
}
unsigned char toBCD(unsigned char value)
{
  return  ((value/10)<<4)+(value%10);
}

int main(void)
{
  unsigned int i=0;
  unsigned int j;
  unsigned int V = 0;
  unsigned int V_avg=0;

  LATD=LATD & 0xFF9F;
  LATB=LATB & 0x80FF;

  TRISB=(TRISB & 0x80FF);
  TRISD=(TRISD & 0xFF9F);
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
    if(i++ % 25 == 0) // 0, 250ms, 500ms, 750ms, ...
    {
      for (j = 0; j < 4; j++)
      {
        V += (int) ((p[j*4]*33+511)/1023);
        V_avg=V/4;
      }
      // Convert analog input (4 samples)
      // Calculate buffer average
      // Calculate voltage amplitude
      // Convert voltage amplitude to decimal
    }
    printInt(V_avg, 10 | 3 << 10);
    putChar(' ');
    send2displays(toBCD(V_avg));
    delay(10);
    // Send voltage value to displays
    // Wait 10 ms (using the core timer)
    IFS1bits.AD1IF = 0;               // Reset AD1IF
  }
  return 0;
}
