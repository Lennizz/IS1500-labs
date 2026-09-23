  # hexmain.S
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
	# Declare hexasc global too so it can be called from another file
	# (e.g. timetemplate.S, or the auto-grader's test harness).
	.globl hex2asc, hexasc

hex2asc:
	li	a0, 10	# test number (from 0 to 15)
	
	addi    sp,sp,-4
	sw      ra,0(sp)
	
	jal	hexasc		# call hexasc
	
	li	a7, 11	# write a0 to stdout
	ecall

	lw      ra,0(sp)
	addi    sp,sp,4
	jr      ra	

hexasc:
	andi 	a0, a0, 0xF 	# Keps value of LSN (last 4 bits) and sets rest to 0s  
	
	li 	t0, 9		# int to compare to
	bgt 	a0, t0, letter	# if a0 bigger than 9, it will be a char (a-f), so jump to char
	
	addi	a0, a0, 0x30	# a0 <= 9 is number, add 48 to get to numbers ascii value
	
	ret			# Same as jr ra, returns
letter:
	addi	a0, a0, 0x37	# a0 > 9, it is a letter, starts 55 so we add it (65 == a)
	
	ret
	

