
@ Syscall defines
.equ SYSCALL_EXIT,     1


        .globl _start
_start:

        @================================
        @ Exit
        @================================
exit:
	mov	r0, #42			@ return 42 as requested for Q2
	mov	r7, #SYSCALL_EXIT	@ put exit syscall number in r7
	swi	0x0			@ exit

