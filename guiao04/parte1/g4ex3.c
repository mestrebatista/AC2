#include <detpic32.h>

void print7Seg(char key)
{

  switch (key)
  {
    case 'a':
    case 'A':
      LATB=(LATB & 0x00FF) | 0x01FF;
      break;
    case 'b':
    case 'B':
      LATB=(LATB & 0x00FF) | 0x02FF;
      break;
    case 'c':
    case 'C':
      LATB=(LATB & 0x00FF) | 0x04FF;
      break;

    case 'd':
    case 'D':
      LATB=(LATB & 0x00FF) | 0x08FF;
      break;
    case 'e':
    case 'E':
      LATB=(LATB & 0x00FF) | 0x10FF;
      break;
    case 'f':
    case 'F':
      LATB=(LATB & 0x00FF) | 0x20FF;
      break;
    case 'g':
    case 'G':
      LATB=(LATB & 0x00FF) | 0x40FF;
      break;
    case '.':
      LATB=(LATB & 0x00FF) | 0x80FF;
      break;
    default:
      LATB=LATB & 0x00FF;;
  }
}

int main(void)
{
  //estados iniciais para os portos RB8 a RB15, RD5 e RD6
  LATB=LATB & 0x00FF;
  LATD=LATD & 0xFF9F;

  TRISB=TRISB & 0x00FF; //portos RB como output
  TRISD=TRISD & 0xFF9F; //portos RD como output

  LATDbits.LATD5=1;
  LATDbits.LATD6=0;

  while(1)
  {
    char key= getChar();
    print7Seg(key);

  }
  return 0;
}
