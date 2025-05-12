;rdi, rsi, rdx, r10, r8, and r9 theese are the argument places
section .data
	message: db "Hello World!", 0xA
	message_len: equ 13

section .text
	global _start
_start:
	mov rax, 65
	push rax
	mov rax, 1; move write syscall into eax
	mov rdi, 1; specify fd 1
	mov rsi, rsp
	mov rdx, 1
	syscall
