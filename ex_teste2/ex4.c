#include <detpic32.h>
void delay(int ms)
{
  for(;ms>0; ms--)
  {
    resetCoreTimer();
    while (readCoreTimer()<20000);
  }
}

void send2display(unsigned char value)
{
  const unsigned char display7Scodes[]={0x3F, 0x06, 0x5B, 0x4F,\
                                        0x66, 0x6D, 0x7D, 0x07,\
                                        0x7F, 0x6F, 0x77, 0x7C,\
                                        0x39, 0x5E, 0x79, 0x71};

  static char displayFlag=0;

  unsigned char digit_low = value & 0x0F;
  unsigned char digit_high =value >> 4;

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
  return (((value/10)<<4)+(value %10));
}

int main(void) {

  LATB=(LATB & 0x80FF);
  LATD=(LATD & 0xFF9F);

  TRISB=(TRISB & 0x80FF);
  TRISD=(TRISD & 0xFF9F);

  char c=0;
  char c_bcd=0;
  int i=0;

  while (1)
  {
    c=inkey();
    if (c=='0') {
      c_bcd=0;
      c=0;
    }
    if (c=='1') {
      c_bcd=1;
      c=0;
    }
    if (c=='2') {
      c_bcd=2;
      c=0;
    }
    if (c=='3') {
      c_bcd=3;
      c=0;
    }
    if(c!='0' || c!='1'|| c!='2'|| c!='3') {
      c_bcd=0xFF;
      do {
        send2display(c_bcd);
        int freq=1/100;
        delay(freq);  //freq de 100Hz
      } while(++i<100); //para uma refresh rate de 50hz
      c=0;
    }

    do {
      send2display(c_bcd);
      int freq=1/100;
      delay(freq);  //freq de 100Hz
    } while(++i<2); //para uma refresh rate de 50hz

  }
  return 0;
}
