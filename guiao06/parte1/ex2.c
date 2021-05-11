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

int main(void)
{
  unsigned int cnt = 0;
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

  EnableInterrupts();   // Global Interrupt Enable
  while(1)
  {
    if(cnt % 25 == 0) // 250 ms (4 samples/second)
    {
      AD1CON1bits.ASAM = 1;   // Start A/D conversion
    }
    send2displays(voltage);// Send "voltage" variable to displays
    cnt++;
    delay(10);//wait 10 ms
  }
  return 0;
}
