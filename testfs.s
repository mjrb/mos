
.section .data
.global testfs
testfs:
	.long fs
fs:
	.long 2
	.long 2
	.long table
table:
	.ascii "testf\0"
	.long endtestf-testf
	.long testf
	.ascii "progm\0"
	.long endprogm-progm
	.long progm
testf:
	.ascii "this is a test file\n"
endtestf:
	.byte 0
progm:
	push %ebp
	movl %esp, %ebp
	
	push %eax
	push %ebx
	push %ecx

	# print args
	movl $0x1, %eax
	movl 0x8(%ebp), %ebx
	int $0xCD
	
	# dump input registers
	movl $0x10, %eax
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

	pop %ebp
	
	ret
endprogm:
	.byte 0

