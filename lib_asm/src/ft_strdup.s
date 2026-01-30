section .text

global ft_strdup

extern malloc
extern ft_strlen
extern strcpy

ft_strdup:
    xor rax, rax
    cmp rdi, 0
    je .ret ; NULL check

    call ft_strlen

    mov rcx, rdi
    mov rdi, rax

    call  malloc

    cmp rax, 0
    je .ret

    mov rdi, rax
    mov rsi, rcx

    call ft_strcpy

    mov rax, rdi

.ret:
    ret

