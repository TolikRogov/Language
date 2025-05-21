section .data
	String db "%d", 10, 0
	_0 dq 0 ;колдун

section .text
	global _start
	extern my_scanf
	extern my_printf

_start:
	;assignment
	push 5
	pop qword [_0] ;колдун

	push rbx
	mov rbx, rbp
	;printf
	;push previous value of stack frame register before new call
	push rbp
	mov rbp, rsp
	push qword [_0] ;колдун
	;call function
	call _2 ;Герой
	;Emit back stack frame register value as before calling function
	mov rsp, rbp
	pop rbp
	;push return value of function
	push rax
	mov rdi, String
	pop rsi
	call my_printf

	;return
	push 0
	pop rdi
	mov rax, 0x3c
	syscall


_2: ;Герой
	push rbx
	mov rbx, rbp
	;if-condition
	push qword [rbx - 8 * 1] ;бродяга
	push 1
	pop r8
	pop r9
	cmp r9, r8
	jne end_if1
		;if-body
		;return
		push 1
		;write return value to register
		pop rax
		pop rbx
		ret

	end_if1:

	;return
	;mul
	push qword [rbx - 8 * 1] ;бродяга
	;push previous value of stack frame register before new call
	push rbp
	mov rbp, rsp
	;sub
	push qword [rbx - 8 * 1] ;бродяга
	push 1
	pop r8
	pop r9
	sub r9, r8
	push r9
	;call function
	call _2 ;Герой
	;Emit back stack frame register value as before calling function
	mov rsp, rbp
	pop rbp
	;push return value of function
	push rax
	pop r8
	pop rax
	mul r8
	push rax
	;write return value to register
	pop rax
	pop rbx
	ret


