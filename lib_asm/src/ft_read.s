default rel
global ft_read
extern __errno_location

ft_read:
    xor rax, rax ; makes it zero
    syscall
    cmp rax, 0
    jge .done

;if syscall returns error
	neg rax
	mov rcx, rax

	call __errno_location wrt ..plt 
	mov [rax], rcx

	mov rax, -1

.done:
    ret