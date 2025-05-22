section .data
	align 16
	temp dq 0
	String db "%d", 10, 0
	_0 dq 0 ;колдун
	_1 dq 0 ;лесник
	_2 dq 0 ;старик
	_4 dq 0 ;герой

section .text
	global _start
	extern my_scanf
	extern my_printf

_start:
	;assignment
	push 1
	pop qword [_0] ;колдун

	;assignment
	push 2
	pop qword [_1] ;лесник

	;assignment
	push 1
	pop qword [_2] ;старик

	push rbx
	mov rbx, rbp
	;if-condition
	push qword [_0] ;колдун
	push 0
	pop r8
	pop r9
	cmp r9, r8
	jne end_if1
		;if-body
		;if-condition
		push qword [_1] ;лесник
		push 0
		pop r8
		pop r9
		cmp r9, r8
		jne end_if2
			;if-body
			;if-condition
			push qword [_2] ;старик
			push 0
			pop r8
			pop r9
			cmp r9, r8
			jne end_if3
				;if-body
				;printf
				push 999
				mov rdi, String
				pop rsi
				call my_printf

				;return
				push 0
				pop rdi
				mov rax, 0x3c
				syscall

			end_if3:

			;printf
			push -999
			mov rdi, String
			pop rsi
			call my_printf

			;return
			push 0
			pop rdi
			mov rax, 0x3c
			syscall

		end_if2:

		;if-condition
		push qword [_2] ;старик
		push 0
		pop r8
		pop r9
		cmp r9, r8
		jne end_if4
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

		end_if4:

		;printf
		push 1
		mov rdi, String
		pop rsi
		call my_printf

		;printf
		;div
		;mul
		push -1
		push qword [_2] ;старик
		pop r8
		pop rax
		mul r8
		push rax
		push qword [_1] ;лесник
		pop r8
		pop rax
		div r8
		push rax
		mov rdi, String
		pop rsi
		call my_printf

		;return
		push 0
		pop rdi
		mov rax, 0x3c
		syscall

	end_if1:

	;if-condition
	push qword [_1] ;лесник
	push 0
	pop r8
	pop r9
	cmp r9, r8
	jne end_if5
		;if-body
		;if-condition
		push qword [_2] ;старик
		push 0
		pop r8
		pop r9
		cmp r9, r8
		jne end_if6
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

		end_if6:

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
		mul r8
		push rax
		push qword [_0] ;колдун
		pop r8
		pop rax
		div r8
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
		push qword [_2] ;старик
		push -1
		pop r8
		pop rax
		mul r8
		push rax
		push qword [_0] ;колдун
		pop r8
		pop rax
		div r8
		push rax
		pop r8
		mov [temp], r8
		movdqa xmm1, [temp]
		sqrtsd xmm0, xmm1
		movq [temp], xmm0
		push qword [temp]
		push -1
		pop r8
		pop rax
		mul r8
		push rax
		mov rdi, String
		pop rsi
		call my_printf

		;return
		push 0
		pop rdi
		mov rax, 0x3c
		syscall

	end_if5:

	;if-condition
	push qword [_2] ;старик
	push 0
	pop r8
	pop r9
	cmp r9, r8
	jne end_if7
		;if-body
		;printf
		push 2
		mov rdi, String
		pop rsi
		call my_printf

		;printf
		push 0
		mov rdi, String
		pop rsi
		call my_printf

		;printf
		;div
		;mul
		push qword [_1] ;лесник
		push -1
		pop r8
		pop rax
		mul r8
		push rax
		push qword [_0] ;колдун
		pop r8
		pop rax
		div r8
		push rax
		mov rdi, String
		pop rsi
		call my_printf

		;return
		push 0
		pop rdi
		mov rax, 0x3c
		syscall

	end_if7:

	;assignment
	push 0
	pop qword [_4] ;герой

	;assignment
	;sub
	;mul
	push qword [_1] ;лесник
	push qword [_1] ;лесник
	pop r8
	pop rax
	mul r8
	push rax
	;mul
	;mul
	push 4
	push qword [_0] ;колдун
	pop r8
	pop rax
	mul r8
	push rax
	push qword [_2] ;старик
	pop r8
	pop rax
	mul r8
	push rax
	pop r8
	pop r9
	sub r9, r8
	push r9
	pop qword [_4] ;герой

	;printf
	push 2
	mov rdi, String
	pop rsi
	call my_printf

	;printf
	;div
	;div
	;add
	;mul
	push qword [_1] ;лесник
	push -1
	pop r8
	pop rax
	mul r8
	push rax
	;sqrt
	push qword [_4] ;герой
	pop r8
	mov [temp], r8
	movdqa xmm1, [temp]
	sqrtsd xmm0, xmm1
	movq [temp], xmm0
	push qword [temp]
	pop r8
	pop r9
	add r9, r8
	push r9
	push 2
	pop r8
	pop rax
	div r8
	push rax
	push qword [_0] ;колдун
	pop r8
	pop rax
	div r8
	push rax
	mov rdi, String
	pop rsi
	call my_printf

	;printf
	;div
	;div
	;sub
	;mul
	push qword [_1] ;лесник
	push -1
	pop r8
	pop rax
	mul r8
	push rax
	;sqrt
	push qword [_4] ;герой
	pop r8
	mov [temp], r8
	movdqa xmm1, [temp]
	sqrtsd xmm0, xmm1
	movq [temp], xmm0
	push qword [temp]
	pop r8
	pop r9
	sub r9, r8
	push r9
	push 2
	pop r8
	pop rax
	div r8
	push rax
	push qword [_0] ;колдун
	pop r8
	pop rax
	div r8
	push rax
	mov rdi, String
	pop rsi
	call my_printf

	;return
	push 0
	pop rdi
	mov rax, 0x3c
	syscall


