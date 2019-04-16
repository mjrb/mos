
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
endprogm:
	.byte 0

