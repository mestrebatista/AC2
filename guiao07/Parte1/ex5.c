#include <detpic32.h>

volatile unsigned char voltage = 0; // Global variable

void delay(int ms) {
  for (; ms > 0; ms--)
  {
    resetCoreTimer();
    while(readCoreTimer()<20000);
  }
}
void send2displays(unsigned char value)
{
  static const char display7Scodes[]={0x3F, 0x06, 0x5B, 0x4F,\
                                      0x66, 0x6D, 0x7D, 0x07,\
                                      0x7F, 0x6F, 0x77, 0x7C,\
                                      0x39, 0x5E, 0x79, 0x71};

  static char displayFlag=0; //variavel estatica para nao perder o seu valor entre estados
  unsigned char digit_low=value & 0x0F;
  unsigned char digit_high=value >>4;

  //toggle displayFlag
  //if flag é 0 entao envia para o display digit_low
  //if flag é 1 entao envia para o display digit_high
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
  return  ((value/10)<<4)+(value%10);
}

void _int_(27) isr_adc(void)
{
  int *p=(int *)(&ADC1BUF0);
  int v=0;
  int i;
  for(i=0; i<8; i++)
  {
    v += (int)((p[i*4]));
  }
  v = ((v/8)*33+511)/(1024-1);

  printInt(v, 16 | 3 << 16);
  putChar(' ');

  voltage=toBCD(v);
  // Calculate buffer average (8 samples)
  // Calculate voltage amplitude
  // Convert voltage amplitude to decimal. Assign it to "voltage"
  IFS1bits.AD1IF = 0;   // Reset AD1IF flag
}


void main(void)
{
  // Configure all (digital I/O, analog input, A/D module, interrupts)
  LATD=LATD & 0xFF9F;
  LATB=LATB & 0x80FF;

  TRISB=(TRISB & 0x80FF);
  TRISD=(TRISD & 0xFF9F);
  // Configure the A/D module and port RB4 as analog input
  TRISBbits.TRISB4 = 1;
  AD1PCFGbits.PCFG4= 0;
  AD1CON1bits.SSRC = 7;
  AD1CON1bits.CLRASAM = 1;
  AD1CON3bits.SAMC = 16;
  AD1CON2bits.SMPI = 8-1;  // 8 consecutive conversion
  AD1CHSbits.CH0SA = 4;    //desiered input analog channel
  AD1CON1bits.ON = 1;

  // Configure interrupt system
  IEC1bits.AD1IE = 1; // enable A/D interrupts
  IPC6bits.AD1IP = 2; // configure priority of A/D interrupts

  //Timer1
  T1CONbits.TCKPS = 3;
  PR1 = 19531;
  TMR1 = 0;
  T1CONbits.TON = 1;

  IPC1bits.T1IP = 2;
  IEC0bits.T1IE = 1;
  IFS0bits.T1IF = 0;

  //Timer3
  T3CONbits.TCKPS = 2;
  PR3 = 49999;
  TMR3 = 0;
  T3CONbits.TON = 1;

  IPC3bits.T3IP = 2;
  IEC0bits.T3IE = 1;
  IFS0bits.T3IF = 0;

  EnableInterrupts();   // Global Interrupt Enable
  while(1);
}

void _int_(4) isr_T1(void)
{
  AD1CON1bits.ASAM = 1;// Start A/D conversion
  IFS0bits.T1IF = 0;  // Reset T1IF flag
}

void _int_(12) isr_T3(void)
{
  send2displays(voltage); // Send "voltage" global variable to displays
  IFS0bits.T3IF = 0;      // Reset T3IF flag
}
