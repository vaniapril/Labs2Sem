.686
.model flat
public _VirusIgnor
.data
_a dd ?
_b dword ?
__0 dd 0

_SimulationDays dword 30
_N dword 10
_DaysOfIncubation dword 5
_DaysOfSymptoms dword 7
_DaysBeforeHelp dword 10
_DeltaDaysBeforeHelp dword 1
_SocialInfection dd 1.0
_SocialInfectionIsolated dd 0.01
_DeathRate dd 0.01

_maxDays dword 17
_dayInf dd 17 dup (0)
_death dd 0

_startDeath dword ?
_dayOfIsolated dword ?

.code
_VirusIgnor proc
	enter 0,0
	mov ecx, _SimulationDays
	finit
	fild _N
	fstp _dayInf[4]
	mov ebx, _DaysOfIncubation
	add ebx, _DaysOfSymptoms
	add ebx, 1
	mov _startDeath, ebx
	
	FOR1:

	;////////DEATH////////
	mov ebx, _startDeath
	fld __0
	FOR_DEATH:
	fld _dayInf[ebx * 4]
	fmul _DeathRate
	fld _dayInf[ebx * 4]
	fsub ST(0), ST(1)
	fstp _dayInf[ebx * 4]
	fadd ST(1), ST(0)
	fstp _a

	add ebx, 1
	cmp ebx, _maxDays
	jne FOR_DEATH
	
	fld _death
	fadd ST(0), ST(1)
	fstp _death
	fstp _a

	;////////NEW_INFECTED////////
	fld __0
	mov ebx, _DaysOfIncubation
	add ebx, _DaysBeforeHelp
	add ebx, 1
	mov _dayOfIsolated, ebx
	mov ebx, 1

	FOR_NEW_INFECTED1:
	fld _dayInf[ebx * 4]
	fmul _SocialInfection
	fadd ST(1), ST(0)
	fstp _a

	add ebx, 1
	cmp ebx, _dayOfIsolated
	jne FOR_NEW_INFECTED1

	FOR_NEW_INFECTED2:
	fld _dayInf[ebx * 4]
	fmul _SocialInfectionIsolated
	fadd ST(1), ST(0)
	fstp _a

	add ebx, 1
	cmp ebx, _maxDays
	jne FOR_NEW_INFECTED2
	
	fstp _dayInf[0]

	;////////NEXTDAY////////
	mov ebx, _dayOfIsolated

	NEXTDAY_FOR1:
	sub ebx, 1
	fld _dayInf[ebx * 4]
	fstp _dayInf[ebx * 4 + 4]
	cmp ebx, 0
	jne NEXTDAY_FOR1

	sub ecx, 1
	cmp ecx, 0
	jne FOR1

	fld _death
	fist _b
	mov eax, _b

	leave
	ret
_VirusIgnor endp
end