[bits 16]
;org 0x7c00

xor ax, ax
mov ds, ax
mov ss, ax
mov es, ax
mov fs, ax
mov gs, ax
mov sp, 0x7c00

mov ax, 0xb800
mov gs, ax
mov ah,0x6
mov bh,0x7
mov cx,0
mov dx,0x184f
int 10h
mov eax, 2
mov ebx, 0
mov ecx, 0;用于记录当前弹射状态
mov si,1
location:
    push eax
    push ebx
    push ecx  ;进栈保存现场
    mov cx, 80
    mul cx ; eax = eax * 80
    add eax, ebx ; eax = eax + ebx
    shl eax, 1 ; eax = eax * 2
    mov di, ax ; 将计算结果保存到 di 寄存器
    mov ah, [si]
    inc si
    mov al,'o'  ;打印字符
    mov [gs:di], ax
    mov ah,0x86
    mov al,0
    mov cx,0x1
    mov dx,0x0
    int 15h
    pop ecx   ;恢复现场
    pop ebx
    pop eax
cmp ecx,0
je d_r
cmp ecx,1
je u_r
cmp ecx,2
je d_l
cmp ecx,3
je u_l
d_r:
    mov ecx,0
    cmp al ,24
    je u_r ;从右下转右上
    cmp bl,79
    je d_l ;从右下转左下
    inc eax
    inc ebx
    jmp location
u_r:
    mov ecx,1
    cmp al ,0
    je d_r ;从右上转右下
    cmp bl,79
    je u_l ;从右下转左上
    dec eax
    inc ebx
    jmp location
d_l:
    mov ecx,2
    cmp al ,24
    je u_l ;从左下转左上
    cmp bl,0
    je d_r ;从左下转右下
    inc eax
    dec ebx
    jmp location
u_l:
    mov ecx,3
    cmp al ,0
    je d_l ;从左上转左下
    cmp bl,0
    je u_r ;从左上转右上
    dec eax
    dec ebx
    jmp location
end:
jmp $

times 510-($-$$) db 0
dw 0xaa55