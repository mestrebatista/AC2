#include <detpic32.h>

void delay(int ms)
{
  for (; ms > 0; ms--)
  {
    resetCoreTimer();
    while(readCoreTimer()<20000);
  }
}

void send2displays(unsigned char value)
{
  unsigned const char display7Scodes[]={0x3F, 0x06, 0x5B, 0x4F,\
                                        0x66, 0x6D, 0x7D, 0x07,\
                                        0x7F, 0x6F, 0x77, 0x7C,\
                                        0x39, 0x5E, 0x79, 0x71};

  unsigned char displayFlag=0;

  unsigned char digit_low=value & 0x0F;
  unsigned char digit_high=value <<4;

  if (displayFlag==0) {
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

int main(void) {

  unsigned int i;
  unsigned int val_adc;
  unsigned int freq;
  unsigned int count;

  LATD=(LATD & 0xFF9F);
  LATB=(LATB & 0x80FF);


  TRISBbits.TRISB4 = 1;
  AD1PCFGbits.PCFG4= 0;
  AD1CON1bits.SSRC = 7;
  AD1CON1bits.CLRASAM = 1;
  AD1CON3bits.SAMC = 16;
  AD1CON2bits.SMPI = 1-1;
  AD1CHSbits.CH0SA = 4;
  AD1CON1bits.ON = 1;


  TRISB=(TRISB & 0x80FF);
  TRISD=(TRISD & 0xFF9F);

  count=20;
  while (1)
  {
    AD1CON1bits.ASAM = 1;
    while( IFS1bits.AD1IF == 0 );


    val_adc=ADC1BUF0;
    freq=1+(val_adc/255);
    count=(count-1)%256;

    i=0;
    do {
      send2displays(count);
      delay(20);
    } while(i++<freq);
    if (count==0) {
      count=20;
    }

    IFS1bits.AD1IF = 0;
  }
  return 0;
}
