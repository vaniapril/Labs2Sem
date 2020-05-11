.686
.model flat
public _BankingDiv
.data
_a dw 2
.code

arg1 EQU [ebp + 8]
arg2 EQU [ebp + 12]

_BankingDiv proc
	enter 0, 0
	mov eax, arg1
	mov ecx, arg2
	cmp ecx, 0
	jne SKIP
	mov eax, 0
	jmp EXIT
	SKIP:

	imul eax, 2
	
	cdq
	idiv ecx
	cmp edx, 0
	jne NEXT
	cdq
	idiv _a
	push eax
	cdq
	idiv _a
	pop eax
	add eax, edx

	jmp EXIT
	NEXT:

	add eax, 1
	cdq
	idiv _a
	EXIT:
	leave
	ret
_BankingDiv endp
end