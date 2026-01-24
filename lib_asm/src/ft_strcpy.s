;The user of the function shall be punished for passing
;incorrect parameters to the function.
;if there is no space in dest to copy src into it
;the program will segfault. and the user should feel sad.

global ft_strcpy

section .text

ft_strcpy:
    xor rax, rax        ; counter

.loop:
    cmp byte [rsi + rax], 0
    je .done
    mov bl, [rsi + rax]
    mov [rdi + rax], bl
    inc rax
    jmp .loop

.done:
    mov byte [rdi + rax], 0
    mov rax, rdi
    ret