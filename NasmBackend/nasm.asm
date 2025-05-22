section .data
	String db "%d", 10, 0
	ScanfString db "%d", 0
	_1 dq 0 ;лесник
	_2 dq 0 ;герой

section .text
	global _start
	extern my_scanf
	extern my_printf

_start:
	push rbx
	mov rbx, rbp
	;assignment
	;input
	mov rdi, ScanfString
	call my_scanf
	push rax
	pop qword [_1] ;лесник

	;assignment
	push 1
	pop qword [_2] ;герой

	while1:
		;while-condition
		push qword [_1] ;лесник
		push 0
		pop r8
		pop r9
		cmp r9, r8
		jle end_while1
		;while-body
		;assignment
		;mul
		push qword [_2] ;герой
		push qword [_1] ;лесник
		pop r8
		pop rax
		imul eax, r8d
		push rax
		pop qword [_2] ;герой

		;assignment
		;sub
		push qword [_1] ;лесник
		push 1
		pop r8
		pop r9
		sub r9, r8
		push r9
		pop qword [_1] ;лесник

		jmp while1
	end_while1:

	;printf
	push qword [_2] ;герой
	mov rdi, String
	pop rsi
	call my_printf

	;return
	push 0
	pop rdi
	mov rax, 0x3c
	syscall


