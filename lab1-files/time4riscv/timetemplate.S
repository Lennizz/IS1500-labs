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
	slli 	a0, a0, 28
	srli 	a0, a0, 28
	
	li 	t0, 9
	ble 	a0, t0, notChar
	
	addi	a0, a0, 7
notChar:
	addi	a0, a0, 48
	
	jr	ra
	
	
delay:
	blez 	a0, exitDelay	# Exits if delay is 0 or less
	
	addi 	a0, a0, -1	# Ticks down 1 ms

	li 	t0, 0		# same as int i in C program, inizlized as 0
	li 	t1, 4711 	# The comparizion number, change it here :)
LDel:
	bge 	t0, t1, delay	# If it is lager than 4711, it will exit the loop and go back to the while loop
	addi	t0, t0, 1	
	j 	LDel		# Loops back
	
	
exitDelay:
	jr 	ra
	


time2string: 
	PUSH s0
	PUSH s1
	PUSH ra


	li 	s0, 16 		# Left shift amount
	li 	s1, 0  		# Loop Counter
	 
loopEx: 
	li 	t0, 2
	beq 	s1, t0, addColon
	
	li 	t0, 5
	beq 	s1, t0, exitLoopEx

	PUSH a0	
	PUSH a1
	sll 	a0, a1, s0	# Gets only the appropriate bits in a1 	
	srli	a0, a0, 28 	# Moves them to LSB and 0s the rest
	
	jal	hexasc		# Convert to HEX
	add 	t1, a0, zero	# Save return value
	POP a1
	POP a0
	
	sb 	t1, 0(a0)	# Store HEX
	
	# Increments all necesary values
	addi 	a0, a0, 1 
	addi 	s0, s0, 4
	addi 	s1, s1, 1
	
	j 	loopEx
	
# Adds a colon on pos 3
addColon:
	li 	t1, 0x3A
	sb 	t1, 0(a0)
	
	addi 	a0, a0, 1 
	addi 	s1, s1, 1
	
	j 	loopEx 
	
# Adds a \0 on pos 5 and exits program
exitLoopEx:
	li 	t1, 0x00
	sb 	t1, 0(a0)
	
	POP ra
	POP s1
	POP s2
	
	jr 	ra
	
 
