global ft_write

ft_write:

	mov rax, 1
	syscall
	cmp rax, 0
	jge .done

.problem:
	neg rax
	mov rcx, rax

	call errno_location
	mov [rax], rcx

	mov rax, -1 ; this way write returns a negative value on error

.done:
	ret