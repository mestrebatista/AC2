      .equ  PRINT_STR,8
      .equ PRINT_INT,6
      .equ READ_INT10,5
      .equ PRINT_INT10,7

      .data
str1: .asciiz "\nIntroduza um numero (sinal e módulo): "
str2: .asciiz "\nValor lido, em base 2: "
str3: .asciiz "\nValor lido, em base 16: "
str4: .asciiz "\nValor lido, em base 10 (unsigned): "
str5: .asciiz "\nValor lido, em base 10 (signed): "

    .text
    .globl main

main:

while1: la $a0,str1
        li $v0,PRINT_STR
        syscall             #printStr(str1);

        li  $v0,READ_INT10
        syscall
        move  $t0,$v0      #value = readInt10();

        la $a0,str2
        li $v0,PRINT_STR
        syscall             #printStr(str2);

        move  $a0,$t0
        li    $a1,2
        li    $v0,PRINT_INT
        syscall             #printInt(value,2);

        la $a0,str3
        li $v0,PRINT_STR
        syscall             #printStr(str3);

        move  $a0,$t0
        li  $a1,16
        li  $v0,PRINT_INT
        syscall             #printInt(value, 16);

        la $a0,str4
        li $v0,PRINT_STR
        syscall             #printStr(str4);

        move  $a0,$t0
        li  $a1,10
        li  $v0,PRINT_INT   #printInt(value, 10);

        la $a0,str5
        li $v0,PRINT_STR
        syscall             #printStr(str5);

        move  $a0,$t0
        li    $v0,PRINT_INT10
        syscall             #printInt10(value);

        j while1

        li  $v0,0           #return 0;
        jr  $ra             #fim do programa
