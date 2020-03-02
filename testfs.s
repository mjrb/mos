
.section .data
.global testfsmeta
testfsmeta:
	.long fs
fs:
	.long 2
	.long 2
	.long table
	.long 0
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
	push %esi
	push %edi

	# print args
	movl $0x2, %eax
	movl $0x1, %ebx
	movl $0x0, %ecx
	movl 0x8(%ebp), %esi
	movl 0xc(%ebp), %edx 	# print arg len
	int $0xCD
	
	# dump input registers
	movl $0x10, %eax
	movl $0x5, %ebx
	movl $0x6, %ecx
	int $0xCD

	# getc
	movl $0x1, %eax
	movl $0x0, %ebx
	movl $0x0, %ecx
	leal -0x4(%esp), %edi
	movl $0x1, %edx
	int $0xCD

	# putc
	movl $0x2, %eax
	movl $0x1, %ebx
	movl $0x0, %ecx
	leal -0x4(%esp), %esi
	movl $0x1, %edx
	int $0xCD

	pop %edi
	pop %esi
	pop %ecx
	pop %ebx
	pop %eax

	pop %ebp
	
	ret
endprogm:
	.byte 0

