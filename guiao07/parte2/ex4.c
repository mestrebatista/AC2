#include <detpic32.h>

volatile int voltage;

void configureAll()
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
}

void setPWM(unsigned int dutyCycle)
{
  if (dutyCycle>=0 && dutyCycle<=100) // duty_cycle must be in the range [0, 100]
  {
    OC1RS = ((PR3+1)*dutyCycle)/100;
  }
}

void main(void)
{
  int dutyCycle;
  configureAll();
  EnableInterrupts(); // Global Interrupt Enable
  while(1)
  {
    // Read RB1, RB0 to the variable "portVal"
    switch(portVal)
    {
    case 0: // Measure input voltage
            // Enable T1 interrupts
            setPWM(0); // LED OFF
            break;
    case 1: // Freeze
            // Disable T1 interrupts
            setPWM(100); // LED ON (maximum bright)
            break;
    default:// LED brigthness control
            // Enable T1 interrupts
            dutyCycle = voltage * 3;
            setPWM(dutyCycle);
            break;
    }
  }
}
