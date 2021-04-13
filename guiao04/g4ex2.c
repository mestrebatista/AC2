#include <detpic32.h>

void delay(int ms)
{
  for(;ms>0;ms--)
  {
    resetCoreTimer();
    while(readCoreTimer()<200000);
  }
}

int main(void)
{
  LATE= LATE & 0xFFF0;
  TRISE= TRISE & 0xFFF0;

  int count=0;

  while(1)
  {
    delay(250);
    LATE=(LATE & 0xFFF0) | count;
    counter++;
  }
  return 0;
}
