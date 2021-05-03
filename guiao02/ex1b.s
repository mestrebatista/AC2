      .equ READ_CORE_TIMER,11
      .equ RESET_CORE_TIMER,12
      .equ PUT_CHAR,6
      .equ PRINT_INT,3
      .data
      .text
      .globl main
main:   li  $t0,0 #counter=0
while:  li  $v0,READ_CORE_TIMER # while (1) {
        syscall                 #readCoreTimer()
        blt $v0,200000,while    #while(readCoreTimer()<200000);

        li  $v0,RESET_CORE_TIMER
        syscall                 #resetCoreTimer();

        addi  $t0,$t0,1         #counter++;

        li  $a1,4
        sll $a1,$a1,16
        ori $a1,$a1,10

        move  $a0,$t0
        li  $v0,PRINT_INT
        syscall

        li  $a0,'\r'
        li  $v0, PUT_CHAR
        syscall

        j while
  
        li  $v0,0               #return 0
        jr  $ra                  #fim do programa
