.set IRQ_BASE, 0x20
.section .text
.extern printf
.extern _ZN16InterruptManager15handleInterruptEhj
.global _ZN16InterruptManager15ignoreInterruptEv

.macro handleIntRequest num
.global _ZN16InterruptManager26handleInterruptRequest\num\()Ev
_ZN16InterruptManager26handleInterruptRequest\num\()Ev:
	movb $\num + IRQ_BASE, (intnum)
	jmp int_bottom
.endm

.macro handleException num
.global _ZN16InterruptManager19handleException\num\()Ev
_ZN16InterruptManager19handleException\num\()Ev:
	movb $\num, (intnum)
	jmp int_bottom
.endm

handleIntRequest 0x00
handleIntRequest 0x01
.global _ZN16InterruptManager26handleInterruptRequest0xCDEv
_ZN16InterruptManager26handleInterruptRequest0xCDEv:
	movb $0xCD , (intnum)
	jmp int_bottom

ditch:
	cli
	pushl sherestr
	call printf
here:
	jmp here

.extern SYS_EAX
.extern SYS_EBX
.extern SYS_ECX
.extern SYS_EDX
.extern SYS_ESI
.extern SYS_EDI
int_bottom:
	movl %eax, (SYS_EAX)
	movl %ebx, (SYS_EBX)
	movl %ecx, (SYS_ECX)
	movl %edx, (SYS_EDX)
	movl %esi, (SYS_ESI)
	movl %edi, (SYS_EDI)
	pusha
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs

	push %esp
	push (intnum)
	call _ZN16InterruptManager15handleInterruptEhj
	# addl $5, esp
	movl %eax, %esp


	popl %gs
	popl %fs
	popl %es
	popl %ds
	popa
	movl (SYS_EAX), %eax
	movl (SYS_EBX), %ebx
	movl (SYS_ECX), %ecx
	movl (SYS_EDX), %edx
	movl (SYS_ESI), %esi
	movl (SYS_EDI), %edi



	#add $4, %esp

_ZN16InterruptManager15ignoreInterruptEv:
	iret
forever:
	hlt
	jmp forever


.data
SYS_ENDEAX:	.word 0
SYS_ENDEBX:	.word 0
SYS_ENDECX:	.word 0

intnum:		.byte 0
sherestr:	.ascii "!!!here\n"
