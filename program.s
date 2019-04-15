.section .text
.global program
program:
	push %eax
	push %ebx
	push %ecx
	
	# dump input registers
	movl $0x4, %eax
	movl $0x5, %ebx
	movl $0x6, %ecx
	int $0xCD

	# getc
	movl $0x3, %eax
	int $0xCD

	#putc
	movl %eax, %ebx
	movl $0x2, %eax
	int $0xCD

	pop %ecx
	pop %ebx
	pop %eax
	
	ret

	# one of the fun parts of writing OS stuff is i can just run
	# stuff in the data segmnet if protection is off
	# like wise i could copy a program into some space in the
	# code segment
	
.section .data
.global dprogram
dprogram:
	push %eax
	push %ebx
	push %ecx
	
	# dump input registers
	movl $0x4, %eax
	movl $0x5, %ebx
	movl $0x6, %ecx
	int $0xCD

	# getc
	movl $0x3, %eax
	int $0xCD

	#putc
	movl %eax, %ebx
	movl $0x2, %eax
	int $0xCD

	pop %ecx
	pop %ebx
	pop %eax
	
	ret
