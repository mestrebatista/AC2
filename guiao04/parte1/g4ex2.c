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
  LATE= LATE & 0xFFF0;    // deixar os outputs a zero
  TRISE= TRISE & 0xFFF0;  // interpreta-se como output os portos

  int count=0;

  while(1)
  {
    delay(250);
    LATE=(LATE & 0xFFF0) | count;
    counter++;
  }
  return 0;
}
