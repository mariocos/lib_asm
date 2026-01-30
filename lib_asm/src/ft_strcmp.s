global ft_strcmp

section .text

ft_strcmp:

    cmp byte [rdi], [rsi]
    jne .diff
    cmp byte [rdi], 0 ; this only can happen if strings are equal and same length
    je .diff
    inc rdi
    inc rsi
    jmp ft_strcmp

.diff:
    mov byte rax, [rdi]
    sub byte [rax], [rsi]
    ret