#include <detpic32.h>


void setPWM(unsigned int dutyCycle)
{
  if (dutyCycle>=0 && dutyCycle<=100) // duty_cycle must be in the range [0, 100]
  {
    OC1RS = ((PR3+1)*dutyCycle)/100;
  }
}


int main(void)
{
  // Configure Timer T3 with interrupts enabled
  T3CONbits.TCKPS = 7;      //
  PR3=39062;
  TMR3 = 0;                 //Reset timer T3 count register
  T3CONbits.TON = 1;        //Enable timer T3 (must be the last command of the
                            //timer configuration sequence)
  OC1CONbits.OCM = 6;       //PWM mode on OCx; fault pin disabled
  OC1CONbits.OCTSEL =1;     //Use timer T2 as the time base for PWM generation
  setPWM(10);
  OC1CONbits.ON = 1;        //Enable OC1 module

  unsigned int dutyCycleVals[8] = {0, 10, 25, 50, 65, 75, 85, 100};

	while(1) {

		int i;
		for ( i = 0; i < 9; ++i)
		{
			i = i%8;

			int j = 0;
			while(j < 5000000) {
				setPWM(dutyCycleVals[i]);
				j++;
			}
		}
	}
  return 0;
}
