# timetemplate.S
# Written 2015 by F Lundevall
# Ported 2024/06 by W Szczerek (from MIPS to RISC-V)
# Copyright abandonded - this file is in the public domain.

#############################################################
# PUSH / POP macros.                                        #
#                                                           #
# RARS and the GNU assembler (used for the DTEK-V board)    #
# use slightly different macro syntax. The file ships with  #
# the RARS-style block active, because Assignments 1-4 are  #
# done in RARS. When you port to the board in Assignment 5, #
# comment out the RARS block below and uncomment the DTEK   #
# block beneath it.                                         #
#############################################################
###################
# Macros for RARS #
###################
#.macro	PUSH (%reg)
#	addi	sp,sp,-4
#	sw	%reg,0(sp)
#.end_macro
#
#.macro	POP (%reg)
#	lw	%reg,0(sp)
#	addi	sp,sp,4
#.end_macro
###################
# Macros for DTEK #
###################
.macro	PUSH reg
	addi sp,sp,-4
	sw \reg,0(sp)
.endm

.macro	POP reg
	lw	\reg,0(sp)
	addi	sp,sp,4
.endm
#############################################################

	.data
	.align 2
mytime:	.word 	0x5957
timstr:	.asciz 	"text more text lots of text\0"
	.text
	# delay is declared global so the auto-grader's test harness (which
	# calls delay directly from a small labmain.S) can resolve the symbol.
	.globl timetemplate, tick, time2string, delay

# Function for displaying a string with a newline at the end	
display_string:	
	li a7,4
	ecall
	li a0, 10
	li a7,11
	ecall
	jr ra
	
timetemplate:
	la	a0, timstr
	jal     display_string
	
	# wait a little
	li	a0, 2		# ms
	jal	delay
	
	# call tick
	la 	a0, mytime
	jal	tick
	
	# call your function time2string
	la	a0, timstr
	la	t0, mytime
	lw	a1, 0(t0)
	jal	time2string
	
	# go back and do it all again
	j	timetemplate

	
# tick: update time pointed to by $a0
tick:	lw	t0, 0(a0)	# get time
	addi	t0, t0, 1	# increase
	andi	t1, t0, 0xf	# check lowest digit
	sltiu	t2, t1, 0xa	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x6	# adjust lowest digit
	
	andi	t1, t0, 0xf0	# check next digit
	sltiu	t2, t1, 0x60	# if digit < 6, okay
	bnez	t2, tiend
	addi	t0, t0, 0xa0	# adjust digit
	
	li	t3, 0xF
	slli	t3, t3, 0x8
	and	t1, t0, t3	# check minute digit
	addi	t3, x0, 0xA
	slli	t3, t3, 0x8
	slt	t2, t1, t3	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x600	# adjust digit - this one's okay, it's lower than 0x7FF 
	
	li	t3, 0xF
	slli	t3, t3, 0xC
	and	t1, t0, t3	# check last digit
	addi	t3, x0, 0x6
	slli	t3, t3, 0xC
	slt	t2, t1, t3	# if digit < 6, okay
	bnez	t2, tiend
	
	li	t3, 0xA
	slli	t3, t3, 0xC
	add	t0, t0, t3	# adjust last digit
tiend:	sw	t0,0(a0)	# save updated result
	jr	ra		# return

#########################################################
# Place for your functions: time2string, hexasc, delay. #
#########################################################


hexasc:
	andi 	a0, a0, 0xF 	# Keps value of LSN (last 4 bits) and sets rest to 0s  
	
	li 	t0, 9		# int to compare to
	bgt 	a0, t0, letter	# if a0 bigger than 9, it will be a char (a-f), so jump to char
	
	addi	a0, a0, 0x30	# a0 <= 9 is number, add 48 to get to numbers ascii value
	
	ret			# Same as jr ra, returns
letter:
	addi	a0, a0, 0x37	# a0 > 9, it is a letter, starts 55 so we add it (65 == a)
	
	ret
	
	
delay:

	blez 	a0, exitDelay	# Exits if delay is 0 or less

	addi 	a0, a0, -1	# Ticks down 1 ms

	li 	t0, 0		# same as int i in C program, inizlized as 0
	li 	t1, 4711 	# The comparizion number, change it here :)
LDel:
	bge 	t0, t1, delay	# If it is lager than 4711, it will exit the loop and go back to the while loop
	addi	t0, t0, 1	
	j 	LDel		# Loops back

	# Entire loop is 3 clock cycles :) 3x4711x2 = 28266 cycles per second

exitDelay:
	jr 	ra
	


time2string:

    PUSH ra		# ra- and s-registers are callee's responsibility, so we save current value to stack
    PUSH s0
    PUSH s1

    li t0, 0xFFFF	# Removes 16 bit that are just noice (we only care about first 2 bytes)
    and a1, a1, t0 

    mv s0, a0		# Move to s-regs to avoid resposibility of a-regs
    mv s1, a1

    srli a0, s1, 12	# Shift 12 bits to the right (logical) to save the MSN as LSN
    jal hexasc 		# in a0 as a argument to hexasc.
    sb a0, 0(s0)	# Stores byte in allocated space, with appropriate offset

    srli a0, s1, 8	# Shift 8 bits to the right (logical) to save the 2nd MSN as LSN
    jal hexasc		# in a0 as a argument to hexasc
    sb a0, 1(s0)	# Stores byte in allocated space, with appropriate offset

    li a0, 0x3a		# 0x3a is the hex value for the ascii char ":"
    sb a0, 2(s0)	# Stores byte in allocated space, with appropriate offset

    srli a0, s1, 4	# Shift 4 bits to the right (logical) to save the 3rd MSN as LSN
    jal hexasc		# in a0 as a argument to hexasc
    sb a0, 3(s0)	# Stores byte in allocated space, with appropriate offset

    add a0, s1, zero	# Add timestr to a0
    jal hexasc 		# in a0 as a argument to hexasc
    sb a0, 4(s0)	# Stores byte in allocated space, with appropriate offset

    li a0, 0x00		# 0x00 is the hex value for the ascii char nullspace (\00)
    sb a0, 5(s0)	# Stores byte in allocated space, with appropriate offset

    POP s1		# Pops all stack saved values to be used by caller
    POP s0
    POP ra

    ret			# Returns :)
