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

ditch:	
	cli
	pushl sherestr
	call printf
here:
	jmp here

	
int_bottom:
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

	#add $4, %esp		

_ZN16InterruptManager15ignoreInterruptEv:
	iret
forever:
	hlt
	jmp forever


.data
intnum:		.byte 0
sherestr:	.ascii "!!!here\n"
