default rel
global ft_write
extern __errno_location

ft_write:

	mov rax, 1
	syscall
	cmp rax, 0
	jge .done

.problem:
	neg rax
	mov rcx, rax

	call __errno_location wrt ..plt 
	mov [rax], rcx

	mov rax, -1 ; this way write returns a negative value on error

.done:
	ret