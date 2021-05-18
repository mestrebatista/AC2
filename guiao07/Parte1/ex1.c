#include <detpic32.h>


int main(void)
{
  // Configure Timer T3 (2 Hz with interrupts disabled)
  T3CONbits.TCKPS = 7;  //1:32 prescaler (i.e. fout_presc = 625 KHz)
  PR3 = 39062;          //Fout = 20MHz / (256 * (39062 + 1)) = 2 Hz
  TMR3 = 0;             //Reset timer T3 count register
  T3CONbits.TON = 1;    //Enable timer T3 (must be the last command of the
                        //timer configuration sequence)
  while(1)
  {
    while(IFS0bits.T3IF==0);// Wait until T3IF = 1
    IFS0bits.T3IF=0;
    putChar('.');
  }
  return 0;
}
