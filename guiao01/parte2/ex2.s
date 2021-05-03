      .equ GET_CHAR,2
      .equ PUT_CHAR,3

      .data
      .text
      .globl main

main:

do: li  $v0,GET_CHAR
    syscall
    move $t0,$v0          #getChar in variable
if: beq $t0,'\n',endif    #start if statement if(c!='\n')

    move $a0,$t0
    li  $v0,PUT_CHAR
    syscall               #putchar(c);

endif: bne $t0,'\n',do

    li  $v0,0             #return 0;
    jr  $ra               #fim do programa
