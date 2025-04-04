%include "head.include"
; you code here
your_if:
    mov eax,  [a1]
    cmp eax, 12
    jl less_than_12
    cmp eax, 24
    jl less_than_24
    shl eax, 4
    mov  [if_flag], eax
    jmp end_if
less_than_12:
    mov eax,  [a1]
    shr eax, 1
    inc eax
    mov  [if_flag], eax
    jmp end_if
less_than_24:
    mov eax, 24
    sub eax,  [a1]
    imul eax,  [a1]
    mov  [if_flag], eax
end_if:
    ; Your code after the if statement
  your_while:
    mov ecx,[a2]                 ;Load a2 into ecx
while_condition:
    cmp ecx, 12                     ;Compare ecx with 12
    jl end_while                    ;Jump to end_while if ecx < 12
    call my_random           ;Call my_random function
    mov [while_flag + ecx - 12],  al
    dec ecx                                 ; Update a2
    jmp while_condition      ; Store the random number in while_flag array
end_while:
    mov[a2], ecx
%include "end.include"
your_function:
    mov ecx, 0
    mov ebx, [your_string]
for_condition:
    mov edx, ecx
    add edx, ebx
    mov al, [edx]
    cmp al, 0
    je end_function
    pushad
    push ax
    call print_a_char
    pop ax
    popad
    inc ecx
    jmp for_condition
end_function:
    ret
