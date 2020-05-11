.686
.model flat
public _SortString
.data
.code

include Macro.inc
include Macro.inc

_GetStringLength proc
	Prologue 0
	mov eax, 0
	mov esi, [ebp + 8]
	FOR1:
	mov bl, [esi]
	cmp bl, 0
	je EXIT
	add esi, 1
	add eax, 1
	jmp FOR1
	EXIT:
	Epilogue
	ret 4
_GetStringLength endp

_CmpStr proc
	Prologue 0
	mov esi, [ebp + 12]
	mov edi, [ebp + 8]

	StoreCommonRegs
	push esi
	call _GetStringLength
	RestoreCommonRegs

	mov ebx, eax

	StoreCommonRegs
	push edi
	call _GetStringLength
	RestoreCommonRegs

	mov ecx, eax
	mov eax, 0
	cmp ecx, ebx
	jl B
	mov eax, 1
	mov ecx, ebx
	B:

	repe cmpsb
	jl L
	jg G
	jmp EXIT
	L:
	mov eax, 1
	jmp EXIT
	G:
	mov eax, 0
	EXIT:
	Epilogue
	ret 8
_CmpStr endp

_SortString proc
	Prologue 0
	mov esi, [ebp + 8]
	mov ebx, [ebp + 12]

	FOR3:

	sub ebx, 1
	cmp ebx, 0
	je EXIT
	mov ecx, ebx
	FOR1:
	CmpWithNext
	cmp eax, 1
	je NEXT1
	SwapWithNext
	NEXT1:
	add esi, 4
	sub ecx, 1
	cmp ecx, 0
	jne FOR1

	sub esi, 8

	sub ebx, 1
	cmp ebx, 0
	je EXIT
	mov ecx, ebx
	FOR2:
	CmpWithNext
	cmp eax, 1
	je NEXT2
	SwapWithNext
	NEXT2:
	sub esi, 4
	sub ecx, 1
	cmp ecx, 0
	jne FOR2

	add esi, 8

	jmp FOR3
	EXIT:

	Epilogue
	ret
_SortString endp
end