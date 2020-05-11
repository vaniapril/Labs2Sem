Vector2D struct
    X dq ?
    Y dq ?
Vector2D ends

PUBLIC LengthVector2D
.data
    _temp dq ?
.code
    LengthVector2D proc    
        LOCAL vect: Vector2D
        finit

        mov rax, qword ptr [rcx]
        mov vect.X, rax 
        mov rax, qword ptr [rcx + 8]
        mov vect.Y, rax
        
        fld vect.X
        fld vect.X
        fmul ST(1), ST(0)
        fstp _temp
        fld vect.Y
        fld vect.Y
        fmul ST(1), ST(0)
        fstp _temp
        fadd ST(1), ST(0)
        fstp _temp
        fsqrt

        fstp _temp
        movsd xmm0, _temp

        RET
    LengthVector2D endp

end