section .data
	_0 dq 0 ;колдун

section .text
	global main
	extern my_scanf
	extern my_printf

main:
	;assignment
	mov r8, 5
	mov [_0], r8 ;колдун

	;printf
	;push previous value of stack frame register before new call
	mov rbp, rsp
	push [0] #колдун
	;call function
	call _2 ;Герой
	;Emit back stack frame register value as before calling function
	mov rsp, rbp
	;push return value of function
	push rax
	call my_printf

	;return
	mov r8, 0
	
;write return value to register
	pop rax

	ret


_2: ;Герой
	pop [rbx+0]
	;if-condition
	push [rbx+0] #бродяга
	mov r8, 1
	jne end_if1:
		;if-body
		;return
		mov r8, 1
		
;write return value to register
		pop rax

		ret

	end_if1:

	;return
	;mul
	push [rbx+0] #бродяга
	;push previous value of stack frame register before new call
	mov rbp, rsp
	;sub
	push [rbx+0] #бродяга
	mov r8, 1
	sub
	;call function
	call _2 ;Герой
	;Emit back stack frame register value as before calling function
	mov rsp, rbp
	;push return value of function
	push rax
	mul
	
;write return value to register
	pop rax

	ret


