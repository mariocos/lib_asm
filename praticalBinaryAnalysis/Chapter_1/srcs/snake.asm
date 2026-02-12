	.file	"snake.c"
	.intel_syntax noprefix
	.text
	.globl	map
	.bss
	.align 16
	.type	map, @object
	.size	map, 25
map:
	.zero	25
	.globl	direction
	.type	direction, @object
	.size	direction, 1
direction:
	.zero	1
	.globl	headI
	.type	headI, @object
	.size	headI, 1
headI:
	.zero	1
	.globl	headJ
	.type	headJ, @object
	.size	headJ, 1
headJ:
	.zero	1
	.globl	snakeSz
	.data
	.type	snakeSz, @object
	.size	snakeSz, 1
snakeSz:
	.byte	1
	.globl	fruitsInMap
	.bss
	.type	fruitsInMap, @object
	.size	fruitsInMap, 1
fruitsInMap:
	.zero	1
	.section	.rodata
.LC0:
	.string	"GAME OVER :(\n"
	.text
	.globl	gameOver
	.type	gameOver, @function
gameOver:
.LFB6:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	edx, 13
	lea	rax, .LC0[rip]
	mov	rsi, rax
	mov	edi, 1
	call	write@PLT
	mov	edi, 1
	call	exit@PLT
	.cfi_endproc
.LFE6:
	.size	gameOver, .-gameOver
	.section	.rodata
.LC1:
	.string	"clear"
	.text
	.globl	clear
	.type	clear, @function
clear:
.LFB7:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	lea	rax, .LC1[rip]
	mov	rdi, rax
	call	system@PLT
	nop
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	clear, .-clear
	.section	.rodata
.LC2:
	.string	"YOU WON!\n"
	.text
	.globl	gametick
	.type	gametick, @function
gametick:
.LFB8:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	movzx	eax, BYTE PTR direction[rip]
	cmp	al, 119
	jne	.L4
	movzx	eax, BYTE PTR headI[rip]
	test	al, al
	je	.L5
	movzx	eax, BYTE PTR headI[rip]
	movzx	eax, al
	lea	edx, -1[rax]
	movzx	eax, BYTE PTR headJ[rip]
	movzx	eax, al
	movsx	rcx, eax
	movsx	rdx, edx
	mov	rax, rdx
	sal	rax, 2
	add	rax, rdx
	lea	rdx, [rax+rcx]
	lea	rax, map[rip]
	add	rax, rdx
	movzx	eax, BYTE PTR [rax]
	movzx	eax, al
	and	eax, 63
	test	eax, eax
	je	.L6
.L5:
	mov	eax, 0
	call	gameOver
.L6:
	movzx	eax, BYTE PTR headI[rip]
	sub	eax, 1
	mov	BYTE PTR headI[rip], al
	jmp	.L7
.L4:
	movzx	eax, BYTE PTR direction[rip]
	cmp	al, 115
	jne	.L8
	movzx	eax, BYTE PTR headI[rip]
	cmp	al, 4
	je	.L9
	movzx	eax, BYTE PTR headI[rip]
	movzx	eax, al
	lea	edx, 1[rax]
	movzx	eax, BYTE PTR headJ[rip]
	movzx	eax, al
	movsx	rcx, eax
	movsx	rdx, edx
	mov	rax, rdx
	sal	rax, 2
	add	rax, rdx
	lea	rdx, [rax+rcx]
	lea	rax, map[rip]
	add	rax, rdx
	movzx	eax, BYTE PTR [rax]
	movzx	eax, al
	and	eax, 63
	test	eax, eax
	je	.L10
.L9:
	mov	eax, 0
	call	gameOver
.L10:
	movzx	eax, BYTE PTR headI[rip]
	add	eax, 1
	mov	BYTE PTR headI[rip], al
	jmp	.L7
.L8:
	movzx	eax, BYTE PTR direction[rip]
	cmp	al, 97
	jne	.L11
	movzx	eax, BYTE PTR headJ[rip]
	test	al, al
	je	.L12
	movzx	eax, BYTE PTR headI[rip]
	movzx	eax, al
	movzx	edx, BYTE PTR headJ[rip]
	movzx	edx, dl
	sub	edx, 1
	movsx	rcx, edx
	movsx	rdx, eax
	mov	rax, rdx
	sal	rax, 2
	add	rax, rdx
	lea	rdx, [rax+rcx]
	lea	rax, map[rip]
	add	rax, rdx
	movzx	eax, BYTE PTR [rax]
	movzx	eax, al
	and	eax, 63
	test	eax, eax
	je	.L13
.L12:
	mov	eax, 0
	call	gameOver
.L13:
	movzx	eax, BYTE PTR headJ[rip]
	sub	eax, 1
	mov	BYTE PTR headJ[rip], al
	jmp	.L7
.L11:
	movzx	eax, BYTE PTR direction[rip]
	cmp	al, 100
	jne	.L7
	movzx	eax, BYTE PTR headJ[rip]
	cmp	al, 4
	je	.L14
	movzx	eax, BYTE PTR headI[rip]
	movzx	eax, al
	movzx	edx, BYTE PTR headJ[rip]
	movzx	edx, dl
	add	edx, 1
	movsx	rcx, edx
	movsx	rdx, eax
	mov	rax, rdx
	sal	rax, 2
	add	rax, rdx
	lea	rdx, [rax+rcx]
	lea	rax, map[rip]
	add	rax, rdx
	movzx	eax, BYTE PTR [rax]
	movzx	eax, al
	and	eax, 63
	test	eax, eax
	je	.L15
.L14:
	mov	eax, 0
	call	gameOver
.L15:
	movzx	eax, BYTE PTR headJ[rip]
	add	eax, 1
	mov	BYTE PTR headJ[rip], al
.L7:
	movzx	eax, BYTE PTR headI[rip]
	movzx	eax, al
	movzx	edx, BYTE PTR headJ[rip]
	movzx	edx, dl
	movsx	rcx, edx
	movsx	rdx, eax
	mov	rax, rdx
	sal	rax, 2
	add	rax, rdx
	lea	rdx, [rax+rcx]
	lea	rax, map[rip]
	add	rax, rdx
	movzx	eax, BYTE PTR [rax]
	cmp	al, 64
	jne	.L16
	movzx	eax, BYTE PTR fruitsInMap[rip]
	sub	eax, 1
	mov	BYTE PTR fruitsInMap[rip], al
	movzx	eax, BYTE PTR snakeSz[rip]
	add	eax, 1
	mov	BYTE PTR snakeSz[rip], al
.L16:
	movzx	ecx, BYTE PTR snakeSz[rip]
	movzx	eax, BYTE PTR headI[rip]
	movzx	eax, al
	movzx	edx, BYTE PTR headJ[rip]
	movzx	edx, dl
	add	ecx, 1
	movsx	rsi, edx
	movsx	rdx, eax
	mov	rax, rdx
	sal	rax, 2
	add	rax, rdx
	lea	rdx, [rax+rsi]
	lea	rax, map[rip]
	add	rax, rdx
	mov	BYTE PTR [rax], cl
	movzx	eax, BYTE PTR snakeSz[rip]
	cmp	al, 25
	jne	.L17
	mov	edx, 9
	lea	rax, .LC2[rip]
	mov	rsi, rax
	mov	edi, 1
	call	write@PLT
	mov	edi, 0
	call	exit@PLT
.L17:
	call	rand@PLT
	movzx	edx, BYTE PTR fruitsInMap[rip]
	movzx	edx, dl
	mov	ecx, 25
	sub	ecx, edx
	movzx	edx, BYTE PTR snakeSz[rip]
	movzx	edx, dl
	sub	ecx, edx
	lea	edi, 1[rcx]
	cdq
	idiv	edi
	mov	ecx, edx
	mov	eax, ecx
	mov	BYTE PTR -1[rbp], al
	mov	BYTE PTR -4[rbp], 0
	mov	BYTE PTR -3[rbp], 0
	jmp	.L18
.L23:
	mov	BYTE PTR -2[rbp], 0
	jmp	.L19
.L22:
	movzx	eax, BYTE PTR -3[rbp]
	movzx	edx, BYTE PTR -2[rbp]
	movsx	rcx, edx
	movsx	rdx, eax
	mov	rax, rdx
	sal	rax, 2
	add	rax, rdx
	lea	rdx, [rax+rcx]
	lea	rax, map[rip]
	add	rax, rdx
	movzx	eax, BYTE PTR [rax]
	movzx	eax, al
	and	eax, 63
	test	eax, eax
	je	.L20
	movzx	edx, BYTE PTR -3[rbp]
	movzx	ecx, BYTE PTR -2[rbp]
	movsx	rdi, ecx
	movsx	rsi, edx
	mov	rax, rsi
	sal	rax, 2
	add	rax, rsi
	lea	rsi, [rax+rdi]
	lea	rax, map[rip]
	add	rax, rsi
	movzx	eax, BYTE PTR [rax]
	lea	esi, -1[rax]
	movsx	rcx, ecx
	movsx	rdx, edx
	mov	rax, rdx
	sal	rax, 2
	add	rax, rdx
	lea	rdx, [rax+rcx]
	lea	rax, map[rip]
	add	rax, rdx
	mov	BYTE PTR [rax], sil
	jmp	.L21
.L20:
	movzx	eax, BYTE PTR -4[rbp]
	cmp	al, BYTE PTR -1[rbp]
	jne	.L21
	movzx	eax, BYTE PTR fruitsInMap[rip]
	cmp	al, 1
	ja	.L21
	movzx	eax, BYTE PTR fruitsInMap[rip]
	add	eax, 1
	mov	BYTE PTR fruitsInMap[rip], al
	movzx	eax, BYTE PTR -3[rbp]
	movzx	edx, BYTE PTR -2[rbp]
	movsx	rcx, edx
	movsx	rdx, eax
	mov	rax, rdx
	sal	rax, 2
	add	rax, rdx
	lea	rdx, [rax+rcx]
	lea	rax, map[rip]
	add	rax, rdx
	mov	BYTE PTR [rax], 64
.L21:
	movzx	eax, BYTE PTR -4[rbp]
	add	eax, 1
	mov	BYTE PTR -4[rbp], al
	movzx	eax, BYTE PTR -2[rbp]
	add	eax, 1
	mov	BYTE PTR -2[rbp], al
.L19:
	cmp	BYTE PTR -2[rbp], 4
	jbe	.L22
	movzx	eax, BYTE PTR -3[rbp]
	add	eax, 1
	mov	BYTE PTR -3[rbp], al
.L18:
	cmp	BYTE PTR -3[rbp], 4
	jbe	.L23
	nop
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	gametick, .-gametick
	.section	.rodata
.LC3:
	.string	"------map-----\n"
.LC4:
	.string	" f "
.LC5:
	.string	"[:)"
.LC6:
	.string	"[S]"
.LC7:
	.string	" . "
.LC8:
	.string	"\n"
	.text
	.globl	printMap
	.type	printMap, @function
printMap:
.LFB9:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	edx, 15
	lea	rax, .LC3[rip]
	mov	rsi, rax
	mov	edi, 1
	call	write@PLT
	mov	DWORD PTR -8[rbp], 0
	jmp	.L25
.L32:
	mov	DWORD PTR -4[rbp], 0
	jmp	.L26
.L31:
	mov	eax, DWORD PTR -4[rbp]
	movsx	rcx, eax
	mov	eax, DWORD PTR -8[rbp]
	movsx	rdx, eax
	mov	rax, rdx
	sal	rax, 2
	add	rax, rdx
	lea	rdx, [rax+rcx]
	lea	rax, map[rip]
	add	rax, rdx
	movzx	eax, BYTE PTR [rax]
	movzx	eax, al
	and	eax, 64
	test	eax, eax
	je	.L27
	mov	edx, 3
	lea	rax, .LC4[rip]
	mov	rsi, rax
	mov	edi, 1
	call	write@PLT
	jmp	.L28
.L27:
	mov	eax, DWORD PTR -4[rbp]
	movsx	rcx, eax
	mov	eax, DWORD PTR -8[rbp]
	movsx	rdx, eax
	mov	rax, rdx
	sal	rax, 2
	add	rax, rdx
	lea	rdx, [rax+rcx]
	lea	rax, map[rip]
	add	rax, rdx
	movzx	eax, BYTE PTR [rax]
	movzx	eax, al
	and	eax, 63
	test	eax, eax
	je	.L29
	mov	eax, DWORD PTR -4[rbp]
	movsx	rcx, eax
	mov	eax, DWORD PTR -8[rbp]
	movsx	rdx, eax
	mov	rax, rdx
	sal	rax, 2
	add	rax, rdx
	lea	rdx, [rax+rcx]
	lea	rax, map[rip]
	add	rax, rdx
	movzx	edx, BYTE PTR [rax]
	movzx	eax, BYTE PTR snakeSz[rip]
	cmp	dl, al
	jne	.L29
	mov	edx, 3
	lea	rax, .LC5[rip]
	mov	rsi, rax
	mov	edi, 1
	call	write@PLT
	jmp	.L28
.L29:
	mov	eax, DWORD PTR -4[rbp]
	movsx	rcx, eax
	mov	eax, DWORD PTR -8[rbp]
	movsx	rdx, eax
	mov	rax, rdx
	sal	rax, 2
	add	rax, rdx
	lea	rdx, [rax+rcx]
	lea	rax, map[rip]
	add	rax, rdx
	movzx	eax, BYTE PTR [rax]
	movzx	eax, al
	and	eax, 63
	test	eax, eax
	je	.L30
	mov	edx, 3
	lea	rax, .LC6[rip]
	mov	rsi, rax
	mov	edi, 1
	call	write@PLT
	jmp	.L28
.L30:
	mov	edx, 3
	lea	rax, .LC7[rip]
	mov	rsi, rax
	mov	edi, 1
	call	write@PLT
.L28:
	add	DWORD PTR -4[rbp], 1
.L26:
	cmp	DWORD PTR -4[rbp], 4
	jle	.L31
	mov	edx, 1
	lea	rax, .LC8[rip]
	mov	rsi, rax
	mov	edi, 1
	call	write@PLT
	add	DWORD PTR -8[rbp], 1
.L25:
	cmp	DWORD PTR -8[rbp], 4
	jle	.L32
	nop
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	printMap, .-printMap
	.globl	main
	.type	main, @function
main:
.LFB10:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	edi, 0
	call	time@PLT
	mov	edi, eax
	call	srand@PLT
	mov	BYTE PTR map[rip], 1
	mov	eax, 0
	call	clear
	mov	eax, 0
	call	printMap
.L37:
	mov	edx, 1
	lea	rax, direction[rip]
	mov	rsi, rax
	mov	edi, 0
	call	read@PLT
	mov	DWORD PTR -4[rbp], eax
	cmp	DWORD PTR -4[rbp], 1
	jne	.L38
	movzx	eax, BYTE PTR direction[rip]
	cmp	al, 100
	je	.L35
	movzx	eax, BYTE PTR direction[rip]
	cmp	al, 115
	je	.L35
	movzx	eax, BYTE PTR direction[rip]
	cmp	al, 97
	je	.L35
	movzx	eax, BYTE PTR direction[rip]
	cmp	al, 119
	jne	.L38
.L35:
	mov	eax, 0
	call	gametick
	mov	eax, 0
	call	clear
	mov	eax, 0
	call	printMap
	jmp	.L37
.L38:
	nop
	jmp	.L37
	.cfi_endproc
.LFE10:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
