.686
.model flat
public _VirusNotIgnor
.data
_a dd ?
__0 dd 0
__1 dd 1.0

_N dword 10
_DaysOfIncubation dword 5
_DaysOfSymptoms dword 7
_DaysBeforeHelp dword 10
_DeltaDaysBeforeHelp dword 1
_DeltaSocialInfection dd 0.1
_SocialInfection dd 1.0
_SocialInfectionIsolated dd 0.01
_DeathRate dd 0.01

_maxDays dword 17
_dayInf dd 17 dup (0)
_death dd 0

_hard dword 0
_startDeath dword ?
_dayOfIsolated dword ?

_DAY1 dword 0
_DAY2 dword 0
_DAY3 dword 0
_DAY4 dword 0
_DAY5 dword 0
_DAY6 dword 0
_DAY7 dword 0
_DAY8 dword 0
_DAY9 dword 0
_DAY10 dword 0
_DAY11 dword 0
_DAY12 dword 0
_DAY13 dword 0
_DAY14 dword 0
_DAY15 dword 0
_DAY16 dword 0
.code

;#############Для режима отладки####################
SHOW MACRO
fld _dayInf[4]
fistp _DAY1
fld _dayInf[8]
fistp _DAY2
fld _dayInf[12]
fistp _DAY3
fld _dayInf[16]
fistp _DAY4
fld _dayInf[20]
fistp _DAY5
fld _dayInf[24]
fistp _DAY6
fld _dayInf[28]
fistp _DAY7
fld _dayInf[32]
fistp _DAY8
fld _dayInf[36]
fistp _DAY9
fld _dayInf[40]
fistp _DAY10
fld _dayInf[44]
fistp _DAY11
fld _dayInf[48]
fistp _DAY12
fld _dayInf[52]
fistp _DAY13
fld _dayInf[56]
fistp _DAY14
fld _dayInf[60]
fistp _DAY15
fld _dayInf[64]
fistp _DAY16
ENDM

_VirusNotIgnor proc
	enter 0,0
	mov ecx, 1
	finit
	fild _N
	fstp _dayInf[4]
	mov ebx, _DaysOfIncubation
	add ebx, _DaysOfSymptoms
	add ebx, 1
	mov _startDeath, ebx
	
	FOR1:
	SHOW
	;////sub DaysBeforeHelp////
	fld _death
	fcom __1
	fstp _a
	FSTSW ax
	SAHF
	jbe SKIP1
	cmp _DaysBeforeHelp, 1
	je SKIP1
	sub _DaysBeforeHelp, 1
	SKIP1:


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
	mov ebx, _maxDays

	NEXTDAY_FOR1:
	sub ebx, 1
	fld __0
	fstp _dayInf[ebx * 4]
	cmp ebx, _dayOfIsolated
	jne NEXTDAY_FOR1

	NEXTDAY_FOR2:
	sub ebx, 1
	fld _dayInf[ebx * 4]
	fstp _dayInf[ebx * 4 + 4]
	cmp ebx, 0
	jne NEXTDAY_FOR2

	;////sub SocialInfection////
	cmp _hard, 0
	jne SUBINF
	mov ebx, _startDeath
	fld _dayInf[ebx * 4]
	ficom __0
	fstp _a
	FSTSW ax
	SAHF
	je SKIP3
	add _hard, 1

	SUBINF:
	fld _SocialInfection
	fcom _DeltaSocialInfection
	fstp _a
	FSTSW ax
	SAHF
	jbe SKIP4

	fld _SocialInfection
	fsub _DeltaSocialInfection
	fstp _SocialInfection
	jmp SKIP3

	SKIP4:
	fld _SocialInfectionIsolated
	fstp _SocialInfection

	SKIP3:

	add ecx, 1

	fld __0
	mov ebx, _maxDays
	FOR2:
	sub ebx, 1
	fadd _dayInf[ebx * 4]
	cmp ebx, 0
	jne FOR2
	fcom __1
	fstp _a
	FSTSW ax
	SAHF
	jae FOR1

	fld _death
	mov eax, ecx
	leave
	ret
_VirusNotIgnor endp
end