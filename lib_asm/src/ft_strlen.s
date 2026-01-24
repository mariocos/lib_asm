; in this function the parameter is protected against
; passing a NULL pointer because i have found i make
; this mistake and as not only user but creator i will
; not punish myself for my dumb mistakes.
; (atleast not always)

global ft_strlen

section .text

ft_strlen: ; function should receive a pointer to a string
	xor rax, rax ; cleans the register to zero

	cmp rdi, 0
	je .done
 
.loop:
	cmp byte [rdi + rax], 0
	je .done
	inc rax
	jmp .loop

.done:
	ret
