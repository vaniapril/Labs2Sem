.686
.model flat
public _Covid
.data
_c dword ?
extern _VirusIgnor:near
extern _VirusNotIgnor:near
.code
_Covid proc
	enter 0,0
	call _VirusIgnor
	mov ecx, [ebp + 8]
	mov [ecx], eax

	call _VirusNotIgnor
	mov ecx, [ebp + 12]
	mov [ecx], eax

	mov ecx, [ebp + 16]
	fistp _c
	mov eax, _c
	mov [ecx], eax

	leave
	ret
_Covid endp
end