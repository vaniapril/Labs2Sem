IFNDEF MACRO_1
MACRO_1 EQU 1

Prologue MACRO N
	push ebp
	mov ebp, esp
	sub esp, N
ENDM

Epilogue MACRO
	mov esp,ebp
	pop ebp
ENDM

StoreCommonRegs MACRO
	push ebx
	push ecx
	push esi
	push edi
ENDM

RestoreCommonRegs MACRO
	pop edi
	pop esi
	pop ecx
	pop ebx
ENDM

CmpWithNext MACRO
	StoreCommonRegs
	push [esi]
	push [esi + 4]
	call _CmpStr
	RestoreCommonRegs
ENDM

SwapWithNext MACRO
	push [esi + 4]
	push [esi]
	pop [esi + 4]
	pop [esi]
ENDM

ENDIF 