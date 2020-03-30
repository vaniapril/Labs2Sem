.686
.model flat
public _IntegrateCotan
.data
__4 dd 4.0
__2 dd 2.0
__0 dd 0
__1 dd 1.0
_a dd ?
.code

_IntegrateCotan proc
	enter 0,0
	mov ecx, [ebp + 8]
	mov _a, ecx

	finit
	fldpi
	fdiv __4
	fild _a
	fdiv ST(1), ST(0)
	fstp _a	
	fldpi
	fdiv __4
	fld ST(0)
	fadd ST(0), ST(2)
	fld __0

	FOR1:
	fld __0
	fadd ST(0), ST(2)
	FPTAN
	fdiv ST(0), ST(1)
	FXCH
	fstp _a

	fld __0
	fadd ST(0), ST(4)
	FPTAN
	fdiv ST(0), ST(1)
	FXCH
	fstp _a

	fadd ST(0), ST(1)
	fdiv __2
	fmul ST(0), ST(5)
	fadd ST(2), ST(0)
	fstp _a
	fstp _a
	fld ST(3)
	fadd ST(2), ST(0)
	fadd ST(3), ST(0)
	fstp _a
	sub ecx, 1
	cmp ecx, 0
	jne FOR1

	leave
	ret
_IntegrateCotan endp
end