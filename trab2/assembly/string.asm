;assembly ia32
;input and output integer
;MAIN FUNCTION IS USED ONLY TO SIMULATE THE FUNCTIONS WORKING

section .data

linebreak db 0dH, 0aH
; user_integer_buffer db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  ;buffer usado no input
; user_integer_buffer2 db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ;buffer usado no output

user_string db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  ;usado apenas na simulacao da funcao
section .bss



section .text

global _start
_start:
    push user_string ; passing argument - address to write string on
    push dword 10   ;passing argument - size of string to read
    call S_INPUT
    ;
    ; pop ebx
    ; mov [user_integer], ebx
    ;-----
    push user_string ; passing argument - address to write string on
    push dword 10   ;passing argument - size of string to read
    call S_OUTPUT
    jmp exit

S_INPUT:
;receive ascii string input
    push ebp        ;creating stack frame
    mov ebp, esp    ;creating stack frame
    push ebx    ;size of string
    push ecx    ;pointer for string and counter
    push edx    ;size of string and string pointer
    ;ask for input
    mov eax, 3
    mov ebx, 0  ;0 = stdin - teclado
    mov ecx, [ebp + 12] ;pointer to string
    mov edx, [ebp + 8]  ;size of string
    int 80H

    mov edx, [ebp + 12] ; move the string pointer to edx
    mov ecx, 0  ;reset counter
    mov ebx, [ebp + 8]  ;move the string size to ebx - avoid segfault if string is max size and has no ENTER
    dec ebx ; dec string size to get the last element index - needed to compare with counter (from 1->x to 0->x-1)
    count_input:
    cmp byte [edx], 0aH    ;check if char is ENTER
    je endcount_input ;if yes, end counting loop
    inc ecx ;increment counter
    cmp ecx, ebx    ;check if counter is at the end of the string
    je endcount_input ;if yes, end counting loop
    inc edx ;get next char
    jmp count_input   ;keep counting
    endcount_input:
    mov eax, ecx    ;move counter to eax - return the number of chars read

    pop edx
    pop ecx
    pop ebx

    pop ebp ;removing stack frame
    ret 8   ;return and remove 2 arguments from stack


S_OUTPUT:
;print integer (ascii string) output
    push ebp    ;creating stack frame
    mov ebp, esp    ;creating stack frame
    push ebx    ;dont push eax because eax is the return value
    push ecx    ;pointer for string and counter
    push edx    ;size of string and pointer to string

    mov ecx, 0  ;reset counter
    mov edx, [ebp + 12] ;pointer to string moved into edx
    mov ebx, [ebp + 8]  ;string size

    count_output:
    cmp byte [edx], 0aH    ;check if char is ENTER
    je endcount_output ;if yes, end counting loop
    inc ecx ;increment counter
    cmp ecx, ebx    ;check if counter is at the end of the string
    je endcount_output ;if yes, end counting loop
    inc edx ;get next char
    jmp count_output   ;keep counting
    endcount_output:
    push ecx    ;store the number of chars in the string

    mov eax, 4
    mov ebx, 1  ;1 = stdout - monitor
    mov edx, ecx
    mov ecx, [ebp + 12]
    int 80H

    ;outputs a linebreak
    mov eax, 4
    mov ebx, 1
    mov ecx, linebreak
    mov edx, 2
    int 80H

    pop eax ;moved ecx into eax - char counter
    pop edx
    pop ecx
    pop ebx

    pop ebp ;removed stack frame
    ret 8 ;returns and removes the arguments received from stack


exit:
    ;exit
    mov eax, 1
    mov ebx, 0
    int 80H
