;assembly ia32 test
;input and output char
;MAIN FUNCTION IS USED ONLY TO SIMULATE THE FUNCTIONS WORKING

section .data

linebreak db 0dH, 0aH
user_char_buffer db 0   ;buffer usado no input

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
    ;ask for input
    mov eax, 3
    mov ebx, 0  ;0 = stdin - teclado
    mov ecx, user_char_buffer
    mov edx, 1
    int 80H

    mov eax, [ecx]  ;moving the char read to the return address on the stack
    mov [ebp + 8], eax  ; ||
    mov eax, 1  ;eax contains the number of elements read - always 1 char

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
    mov eax, 4
    mov ebx, 1
    mov ecx, linebreak
    mov edx, 2
    int 80H

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
