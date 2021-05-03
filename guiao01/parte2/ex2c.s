.equ	INKEY,1
.equ	PUT_CHAR,3

.data
.text
.globl main

main:
do:
while:	li	$v0,INKEY
    syscall
    move	$t0,$v0        #c=inkey();

    beq $t0,$0,while     #while((c=inkey())==0);
if: beq $t0,'\n',endif
    move  $a0,$t0

    li  $v0,PUT_CHAR
    syscall             #putChar(c);

endif: bne $t0,'\n', do #while(c!='\n');

    li  $v0,0           #return 0
    jr  $ra             #fim do programa
