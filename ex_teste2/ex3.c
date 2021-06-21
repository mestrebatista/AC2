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

  char c=0;

  while (1)
  {
    c=inkey();
    if (c=='0') {
      LATEbits.LATE0=1;
      c=0;
    }
    if (c=='1') {
      LATEbits.LATE1=1;
      c=0;
    }
    if (c=='2') {
      LATEbits.LATE2=1;
      c=0;
    }
    if (c=='3') {
      LATEbits.LATE3=1;
      c=0;
    }
    if(c!='0' || c!='1'|| c!='2'|| c!='3') {
      LATE=(LATE & 0xFFF0) | 0x000F;
      delay(1000);
      LATE=(LATE & 0xFFF0);
    }
    
  }
  return 0;
}
