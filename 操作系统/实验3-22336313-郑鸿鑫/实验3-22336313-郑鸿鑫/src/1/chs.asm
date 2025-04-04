org 0x7c00
[bits 16]
xor ax, ax ; eax = 0
; 初始化段寄存器, 段地址全部设为0
mov ds, ax
mov ss, ax
mov es, ax
mov fs, ax
mov gs, ax
; 初始化栈指针
mov sp, 0x7c00
mov bx, 0x7e00            ; 目标内存地址
mov ah, 0x02          ; 功能号：读取扇区
mov al, 5             ; 读取扇区数
mov ch, 0             ; 柱面号
mov cl, 2             ; 扇区号
mov dh, 0             ; 磁头号
int 0x13                   ; 调用BIOS中断
jmp 0x0000:0x7e00        ; 跳转到bootloader
jmp $ ; 死循环
times 510 - ($ - $$) db 0
db 0x55, 0xaa