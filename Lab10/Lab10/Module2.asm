Vector3D struct
    X dq ?
    Y dq ?
    Z dq ?
Vector3D ends

PUBLIC SumVector3D
.data
    _x dq ?
.code
    SumVector3D proc    
       LOCAL result: Vector3D
       MOV rax, qword ptr [rcx]
       MOV result.X, rax
       MOV rax, qword ptr [rcx + 8]
       MOV result.Y, rax
       MOV rax, qword ptr [rcx + 16]
       MOV result.Z, rax

       MOV rax, qword ptr [rdx]
       ADD result.X, rax
       MOV rax, qword ptr [rdx + 8]
       ADD result.Y, rax
       MOV rax, qword ptr [rdx + 16]
       ADD result.Z, rax

       lea rax, result

       RET
    SumVector3D endp
end