;assembly ia32 test
;input and output char
;MAIN FUNCTION IS USED ONLY TO SIMULATE THE FUNCTIONS WORKING

section .data


user_char db 0  ;usado apenas para a simulacao da funcao
section .bss



section .text

global _start
_start:
    push ebx ; saving space for return
    call C_INPUT

    pop ebx
    mov [user_char], ebx
    ;-----
    mov eax, [user_char]
    push eax    ;pushing argument
    call C_OUTPUT
    jmp exit

C_INPUT:
;receive ascii char input
    push ebp        ;creating stack frame
    mov ebp, esp    ;creating stack frame
    push ebx    ;dont push eax because eax is the return value
    push ecx    ;counter of elements - always 1
    push edx    ;pointer for char string

    push word 0 ;reserve space for input buffer
    ;ask for input
    mov eax, 3
    mov ebx, 0  ;0 = stdin - teclado
    mov ecx, esp
    add ecx, 2
    mov edx, 1
    int 80H
    
    mov eax, [ecx]  ;moving the char read to the return address on the stack
    mov [ebp + 8], eax  ; ||
    
    dec ecx ;get to the second byte of memory on the buffer - used to read until enter
    check_extra_char_enter:
    mov eax, 3
    mov ebx, 0  ;0 = stdin - teclado
    mov edx, 1
    int 80H
    cmp byte [ecx], 0aH    ;check if char is ENTER
    jne check_extra_char_enter
    
    mov eax, 1  ;eax contains the number of elements read - always 1 char

    pop cx ; removes the buffer from top of stack

    pop edx
    pop ecx
    pop ebx

    pop ebp ;remove the stack frame

    ret


C_OUTPUT:
;print ascii char output
    push ebp    ;creating stack frame
    mov ebp, esp    ;creating stack frame
    push ebx    ;dont push eax because eax is the return value
    push ecx    ;digit counter
    push edx    ;rest of division - number to convert


    mov eax, 4   ;interruption
    mov ebx, 1  ;stdout - monitor
    mov ecx, ebp   ;char to print - passed as argument through stack
    add ecx, 8  ;||
    mov edx, 1 ;1 char to print
    int 80H

    ;outputs a linebreak
    push word 0d0aH ;add linebreak to stack
    mov eax, 4
    mov ebx, 1
    mov ecx, esp
    mov edx, 2
    int 80H
    add esp, 2 ;remove linebreak from stack

    mov eax, 1    ;number of chars printed - always 1 char
    pop edx
    pop ecx
    pop ebx

    pop ebp ;remove stack frame

    ret 4   ;return and remove argument from stack


exit:
    ;exit
    mov eax, 1
    mov ebx, 0
    int 80H
