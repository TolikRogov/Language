section .data
	align 16
	temp dq 0
	String db "%d", 10, 0
	ScanfString db "%d", 0
	_0 dq 0 ;колдун
	_1 dq 0 ;лесник
	_2 dq 0 ;старик

section .text
	global _start
	extern my_scanf
	extern my_printf

_start:
	;assignment
	;input
	mov rdi, ScanfString
	call my_scanf
	push rax
	pop qword [_0] ;колдун

	;assignment
	;input
	mov rdi, ScanfString
	call my_scanf
	push rax
	pop qword [_1] ;лесник

	;assignment
	;input
	mov rdi, ScanfString
	call my_scanf
	push rax
	pop qword [_2] ;старик

	push rbx
	mov rbx, rbp
	;if-condition
	push qword [_1] ;лесник
	push 0
	pop r8
	pop r9
	cmp r9, r8
	jne end_if1
		;if-body
		;if-condition
		push qword [_2] ;старик
		push 0
		pop r8
		pop r9
		cmp r9, r8
		jne end_if2
			;if-body
			;printf
			push 1
			mov rdi, String
			pop rsi
			call my_printf

			;printf
			push 0
			mov rdi, String
			pop rsi
			call my_printf

			;return
			push 0
			pop rdi
			mov rax, 0x3c
			syscall

		end_if2:

		;printf
		push 2
		mov rdi, String
		pop rsi
		call my_printf

		;printf
		;sqrt
		;div
		;mul
		push qword [_2] ;старик
		push -1
		pop r8
		pop rax
		imul r8
		push rax
		push rax
		mov rdi, String
		pop rsi
		call my_printf
		push qword [_0] ;колдун
		pop r8
		pop rax
		idiv r8
		push rax
		pop r8
		mov [temp], r8
		movdqa xmm1, [temp]
		sqrtsd xmm0, xmm1
		movq [temp], xmm0
		push qword [temp]
		mov rdi, String
		pop rsi
		call my_printf

		;printf
		;mul
		;sqrt
		;div
		;mul
		; push qword [_2] ;старик
		; push -1
		; pop r8
		; pop rax
		; mul r8
		; push rax
		; push qword [_0] ;колдун
		; pop r8
		; pop rax
		; div r8
		; push rax
		; pop r8
		; mov [temp], r8
		; movdqa xmm1, [temp]
		; sqrtsd xmm0, xmm1
		; movq [temp], xmm0
		; push qword [temp]
		; push -1
		; pop r8
		; pop rax
		; mul r8
		; push rax
		; mov rdi, String
		; pop rsi
		;call my_printf

		;return
		push 0
		pop rdi
		mov rax, 0x3c
		syscall

	end_if1:


