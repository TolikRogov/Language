#assignment
#input
IN
POP [0] #колдун

#bx - amount of global variables default
PUSH 1
POP BX
PUSH BX

#main
CALL Путник:
HLT

Путник:
	#printf
	#push previous value of stack frame register before new call
	PUSH BX

	PUSH [0] #колдун
	#stack frame register change value
	PUSH BX
	PUSH 0
	ADD
	POP BX

	#call function
	CALL Герой:
	#Emit back stack frame register value as before calling function
	POP BX

	#push return value of function
	PUSH AX
	OUT

	#return
	PUSH 0
	
#write return value to register
	POP AX

	RET


Герой:
	POP [BX+0]
	#if-condition
	PUSH [BX+0] #бродяга
	PUSH 1
	JNE end_if1:
		#if-body
		#return
		PUSH 1
		
#write return value to register
		POP AX

		RET

	end_if1:

	#return
	#mul
	PUSH [BX+0] #бродяга
	#push previous value of stack frame register before new call
	PUSH BX

	#sub
	PUSH [BX+0] #бродяга
	PUSH 1
	SUB
	#stack frame register change value
	PUSH BX
	PUSH 1
	ADD
	POP BX

	#call function
	CALL Герой:
	#Emit back stack frame register value as before calling function
	POP BX

	#push return value of function
	PUSH AX
	MUL
	
#write return value to register
	POP AX

	RET


