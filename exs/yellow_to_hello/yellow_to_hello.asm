global _start

section .data
	str: db "yellow", 0xa

section .text
_start:
	mov rax, 1
	mov rdi, 1
	mov rsi, str
	mov rdx, 7
	syscall
	mov [str], byte 'H'
	mov [str + 5], byte '!'
	mov rax, 1
	mov rdi, 1
	mov rsi, str
	mov rdx, 7
	syscall
	mov rax, 60
	mov rdi, 0
	syscall

