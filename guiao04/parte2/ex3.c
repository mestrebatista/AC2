#include <detpic32.h>

void delay(int ms)
{
  for (; ms > 0;ms--)
  {
    resetCoreTimer();
    while(readCoreTimer()<200000);
  }
}

void send2displays(unsigned char value)
{
  static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F,\
                                        0x66, 0x6D, 0x7D, 0x07,\
                                        0x7F, 0x6F, 0x77, 0x7C,\
                                        0x39, 0x5E, 0x79, 0x71};
  static char displayFlag = 0; // static variable: doesn't loose its
                               // value between calls to function
  unsigned char digit_low = value & 0x0F;
  unsigned char digit_high = value >> 4;
  // if "displayFlag" is 0 then send digit_low to display_low
  // else send digit_high to didplay_high
  // toggle "displayFlag" variable
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

int main(void)
{
  unsigned char counter=0;
  unsigned int i;

  LATB=LATB & 0x80FF;
  LATD=LATD & 0xFF9F;

  TRISB=(TRISB & 0x80FF);
  TRISD=(TRISD & 0xFF9F);
  while (1)
  {
    i = 0;
    do
    {
      send2displays( counter );
      delay(50);              // wait 50 ms
    } while(++i < 4);
    counter=(counter+1)%256;  // increment counter (mod 256)
  }
  return 0;
}
