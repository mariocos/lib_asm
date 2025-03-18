; in this exercise i want to try to create a loop
;
;global _start
;
;section .text
;_start:
;	mov rax, 60
;	mov rdi, 42
;	jmp skip ;this is a jump operation to a label "skip"
;	mov rdi, 24 ;this wont happen because of jmp
;skip:
;	syscall

global _start:

section .text

_start:
	mov rbx, 0
	mov rcx, 0
_loop_start:
	cmp rbx, 10
	jg skip

	add rbx, 1
	add rcx, 2
	cmp rbx, 10
	jl _loop_start
skip:
	mov rax, 60
	mov rdi, rcx
	syscall
;this is my first ever loop in assembly