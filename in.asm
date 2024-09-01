SECTION .data
msg     db "Hello, world!",0xa  
len     equ $ - msg
SECTION .text
global _start            ; the program entry point
_start:
        push dword len
        push dword msg
        push dword 1     ; 1 is the file descriptor of stdout
        mov eax, 4       ; 4 is the 'write' syscall
        push eax         ; we must leave an extra dword on the stack
        int 0x80         ; call to the kernel
        add esp, 16      ; clean up the stack
        push dword 0     ; 0 is the exit code (success)
        mov eax, 1       ; 1 is the '_exit' syscall
        push eax         ; extra dword on the stack
        int 0x80         ; call to the kernel
                         ; no cleanup - we will never return