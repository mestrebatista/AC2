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

  unsigned char key;
  unsigned int i;
  LATD=(LATD & 0xFF9F);
  LATB=(LATB & 0x80FF);

  TRISB=(TRISB & 0x80FF);
  TRISD=(TRISD & 0xFF9F);

  while (1)
  {
    i=0;
    key=getChar();
    if (key=='0') {
      do {
        send2displays(0x00);
        delay(10);
      } while(i++<100);
    }
    if (key=='1') {
      do {
        send2displays(0x01);
        delay(10);
      } while(i++<100);
    }
    if (key=='2') {
      do {
        send2displays(0x02);
        delay(10);
      } while(i++<100);
    }
    if (key=='3') {
      do {
        send2displays(0x03);
        delay(10);
      } while(i++<100);
    }
    if(key<'0' && key>'3')
    {
      do {
        send2displays(0xFF);
        delay(1000);
        LATDbits.LATD5=0;
        LATDbits.LATD6=0;
      } while(i++<100);
    }
  }
  return 0;
}
