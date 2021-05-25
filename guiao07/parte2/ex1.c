#include <detpic32.h>

int main(void)
{
  // Configure Timer T3 with interrupts enabled
  T3CONbits.TCKPS = 7;      //
  PR3 = 39062;              //
  TMR3 = 0;                 //Reset timer T3 count register
  T3CONbits.TON = 1;        //Enable timer T3 (must be the last command of the
                            //timer configuration sequence)
  OC1CONbits.OCM = 6;       //PWM mode on OCx; fault pin disabled
  OC1CONbits.OCTSEL =1;     //Use timer T2 as the time base for PWM generation
  OC1RS = ((PR3+1)*25)/100; //Ton constant
  OC1CONbits.ON = 1;        //Enable OC1 module

  while(1)
  {
    while(IFS0bits.T3IF==0);
    putChar('.');
    IFS0bits.T3IF=0;          // Reset T3 interrupt flag
  }
  return 0;
}
