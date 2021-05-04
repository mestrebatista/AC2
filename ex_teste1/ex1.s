      .data
      .equ SFR_BASE_HI, 0xBF88
      .equ TRISE, 0x6100
      .equ PORTE, 0x6110
      .equ LATE, 0x6120
      .equ getChar,2
      .equ RESET_CORE_TIMER,12
      .equ READ_CORE_TIMER,11

      .text
      .globl main

main:
      lui	$t1,SFR_BASE_HI
      lw	$t2,TRISE($t1)
      andi	$t2,$t2,0xFFE0
      sw	$t2,TRISE($t1)

loop: li $v0,getChar
      syscall

if0:  bne $v0,48,if1
      lw	$t2,LATE($t1)
      ##andi	$t2,$t2,0xFFE0
      ori	$t2,$t2,0x0001
      sw	$t2,LATE($t1)
      j loop
if1:  bne $v0,49,if2
      lw	$t2,LATE($t1)
      ##andi	$t2,$t2,0xFFE0
      ori	$t2,$t2,0x0002
      sw	$t2,LATE($t1)
      j loop
if2:  bne $v0,50,if3
      lw	$t2,LATE($t1)
      ##andi	$t2,$t2,0xFFE0
      ori	$t2,$t2,0x0004
      sw	$t2,LATE($t1)
      j loop
if3:  bne $v0,51,if4
      lw	$t2,LATE($t1)
      ##andi	$t2,$t2,0xFFE0
      ori	$t2,$t2,0x0008
      sw	$t2,LATE($t1)
      j loop
if4:  ble $v0,52,endif
      lw	$t2,LATE($t1)
      ##andi	$t2,$t2,0xFFE0
      ori	$t2,$t2,0x0010
      sw	$t2,LATE($t1)

      li  $a0,2000
      jal delay

      lw	$t2,LATE($t1)
      andi	$t2,$t2,0xFFE0
      sw	$t2,LATE($t1)

endif:j loop
      jr $ra

#função delay aplicada
delay:
for:  ble  $a0,0,endfor
      addi $a0,$a0,-1
      li  $v0,RESET_CORE_TIMER
      syscall
while:li $v0,READ_CORE_TIMER
      syscall
      bge $v0,20000, for
      j while
endfor:jr $ra
