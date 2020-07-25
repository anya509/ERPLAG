section .data        ; This section is for initialized variables

        readformat: db "INPUT: Enter an integer value",10,0 
        writeformat: db "OUTPUT: ",0 
        newLine db 0AH,0DH,0  ;for new line
        format: db "%ld",0
        formatArray: db "%ld ",0
        truem db "true",10,0
        falsem db "false",10,0
        enterint db "INPUT: Enter an integer value",10,0
        enterbool db "INPUT: Enter a boolean (0 or 1) value",10,0
        entereal db "INPUT: Enter a real value",10,0
        enterintArr db "Enter %d elements of integer type for range %d and %d",10,0
        enterboolArr db "Enter %d elements of boolean (0 or 1) type for range %d and %d",10,0
        enterrealArr db "Enter %d elements of real type for range %d and %d",10,0
        bounderror db "Run time error: Array index out of bounds",10,0

section .bss        ; This section is for uninitialized locally declared variables

        x_driver_0:         resq  1
        y_driver_0:         resq  1
        z_driver_0:         resq  1
        a_driver_0:         resq  1
        b_driver_0:         resq  1
        c_driver_0:         resq  1
        loopVar:        resq  1
        nextAddress:        resq  1

section .text        ; Code Section

        global main
        extern scanf
        extern printf
        extern exit
        default rel

main :
        push        rbx
        mov         rax,nextAddress
        add         rax,8
        mov         qword[nextAddress],rax
        mov         rax, 5
        push         rax
        pop         rax
        mov         qword[a_driver_0],rax
        mov         rax, 9
        push         rax
        pop         rax
        mov         qword[b_driver_0],rax
        lea        rdi, [enterint]
        mov        rsi,0
        xor        rax,rax
        call        printf
        lea        rdi, [format]
        lea        rsi, [x_driver_0]
        xor        rax, rax
        call        scanf
        lea        rdi, [enterint]
        mov        rsi,0
        xor        rax,rax
        call        printf
        lea        rdi, [format]
        lea        rsi, [y_driver_0]
        xor        rax, rax
        call        scanf
        push         qword[x_driver_0]
        push         qword[y_driver_0]
        push         qword[b_driver_0]
        pop         rbx
        pop         rax
        imul         rbx
        push         rax
        push         qword[a_driver_0]
        push         qword[b_driver_0]
        pop         rbx
        pop         rax
        sub         rax, rbx
        push         rax
        push         qword[y_driver_0]
        pop         rbx
        pop         rax
        imul         rbx
        push         rax
        push         qword[a_driver_0]
        mov         rax, 2
        push         rax
        pop         rbx
        pop         rax
        imul         rbx
        push         rax
        pop         rbx
        pop         rax
        add         rax, rbx
        push         rax
        pop         rbx
        pop         rax
        add         rax, rbx
        push         rax
        pop         rbx
        pop         rax
        add         rax, rbx
        push         rax
        push         qword[b_driver_0]
        push         qword[x_driver_0]
        pop         rbx
        pop         rax
        imul         rbx
        push         rax
        pop         rbx
        pop         rax
        sub         rax, rbx
        push         rax
        pop         rax
        mov         qword[z_driver_0],rax
        lea        rdi,[writeformat]
        mov        rsi,0
        xor        rax,rax
        call        printf
        lea        rdi, [format]
        mov        rsi, [z_driver_0]
        xor        rax, rax
        call        printf
        lea        rdi,[newLine]
        mov        rsi,0
        xor        rax,rax
        call        printf
        pop rbx
        ret
.boundError:
        lea        rdi,[bounderror]
        mov        rsi,0
        xor        rax,rax
        call        printf
        mov         rax,60
        xor         rdi,rdi
        call         exit
        pop rbx
