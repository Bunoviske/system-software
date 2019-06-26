;assembly ia32
;input and output integer
;MAIN FUNCTION IS USED ONLY TO SIMULATE THE FUNCTIONS WORKING

section .data


user_integer db 0, 0, 0, 0  ;usado apenas na simulacao da funcao
section .bss



section .text

global _start
_start:
    push ebx ; saving space for return
    call H_INPUT

    pop ebx
    mov [user_integer], ebx
    ;-----
    mov eax, [user_integer]
    push eax    ;pushing argument
    call H_OUTPUT
    jmp exit

H_INPUT:
;receive integer (ascii string) input
    push ebp        ;creating stack frame
    mov ebp, esp    ;creating stack frame
    push ebx    ;dont push eax because eax is the return value
    push ecx    ;counter of elements
    push edx    ;pointer for char string

    push dword 0  ;reserve space (8 bytes) to use as buffer
    push dword 0

    push edi
    push word 0 ;reserve buffer to read the extra chars
    mov ecx, esp    ;move the buffer to ecx
    xor esi, esi    ;reset counter
    mov edi, esp ;pointer to string
    add edi, 14

    ;ask for input
    get_input_charbychar:
    mov eax, 3
    mov ebx, 0  ;0 = stdin - teclado
    mov edx, 1
    int 80H
    cmp byte [ecx], 0aH    ;check if char is ENTER
    je finish_input ;if enter - finish input
    breakpoint:
    mov eax, [ecx]  ;move the char into the string
    mov byte [edi + esi], al
    inc esi ;increment char counter
    ; dec edi ; get next char address
    cmp esi, 8  ;check if string is full --> counter == string size
    je check_extra_char_enter   ;if yes = string full but still no enter, read and trash next chars until enter
    jmp get_input_charbychar    ;keep getting input

    check_extra_char_enter: ;read char and throw it away until enter
    mov eax, 3
    mov ebx, 0  ;0 = stdin - teclado
    mov edx, 1
    int 80H
    cmp byte [ecx], 0aH    ;check if char is ENTER
    jne check_extra_char_enter

    finish_input:   ;finish getting inputs

    add esp, 2  ;remove buffer
    pop edi ;pops edi back



    xor eax, eax ;reset the accumulator to zero
    mov edx, esp   ;put address in edx
    add edx, 8
    push dword 16 ;stack the value 16 to multiply
    xor ecx, ecx ; zero the counter


convert_charint_hex:
    cmp ecx, 8  ;avoids acessing an exra element
    je finish_charint_hex
    movzx ebx, byte [edx] ;get char
    inc edx ;prepare next char
    inc ecx ;increment counter

    cmp ebx, '0'    ;check if number
    jb finish_charint_hex
    cmp ebx, '9'    ;check if number
    jbe convert_char_int
    cmp ebx, 'A'    ;check if number as hex upper
    jb finish_charint_hex   ;already jumped if between 0-9
    cmp ebx, 'F'    ;check if number as hex upper
    jbe convert_hex_int_upper
    cmp ebx, 'a'    ;check if number as hex lower
    jb finish_charint_hex
    cmp ebx, 'f'    ;check if umber as hex lower
    jbe convert_hex_int_lower   ;already jumped if other cases
    jmp finish_charint_hex  ;checked everything - not a valid char
    ;if reorganized, could have spared one extra jump, but this way the code is more readable
    ;the "switch" follows the same logic and pattern throughout its code

    convert_hex_int_upper:    ;if digit is hex char - convert to decimal value
    sub ebx, 'A'    ;convert to integer number
    add ebx, 10
    jmp after_check_charint_hex
    convert_hex_int_lower:
    sub ebx, 'a'    ;convert to integer number
    add ebx, 10
    jmp after_check_charint_hex

    convert_char_int:   ;if digit is decimal char - convert to decimal value
    sub ebx, '0' ;convert to integer number

    after_check_charint_hex:
    push edx    ;save edx before ultiplication
    mul dword [esp+4] ;multiply eax by 16 = get ready for next digit
    pop edx ;no use for edx, maximum value of multiplication fits eax
    add eax, ebx    ;adds to acumulator value
    jmp convert_charint_hex ;jump until finished

finish_charint_hex:
    dec ecx ; counter will have 1 extra from the ending char
    add esp, 12  ;remove 16  and buffer from top of stack
finish_after_check_charint_hex:
    mov[ebp + 8], eax   ;no arguments - return value is ebp + 8 bytes pushed by call and frame creation (return and ebp)
    mov eax, ecx    ;move counter into eax - number of characters input

    pop edx
    pop ecx
    pop ebx

    pop ebp ;removing stack frame

    ret


H_OUTPUT:
;print integer (ascii string) output
    push ebp    ;creating stack frame
    mov ebp, esp    ;creating stack frame
    push ebx    ;dont push eax because eax is the return value
    push ecx    ;digit counter
    push edx    ;rest of division - number to convert

    push dword 0    ;reserve space to use as buffer
    push dword 0



    mov eax, [ebp + 8]   ;number - passed as argument through stack
    mov ebx, esp   ;string
    add ebx, 8
    mov ecx, 0  ;digit counter
    push dword 16 ;stack the value 16 to divide

convert_intchar_hex:
    mov edx, 0    ;zero edx (rest of division - number to convert)
    div dword [esp + ecx * 2] ;divide by 16
    cmp edx, 10
    jge convert_int_hex
    add edx, '0'    ;convert digit to ascii char
    jmp after_check_hex
    convert_int_hex:
    add edx, 'A'
    sub edx, 10
    after_check_hex:
    push dx ;stack number converted to ascii - stack works with a minimum 16 bits
    inc ecx ;count number of digits converted
    cmp eax, 0  ;check if eax is 0 or if there is still stuff to divide
    jne convert_intchar_hex ;jumps while not finished



output_intchar_hex:
    unstack_result:
    pop dx  ;pops the chars converted to get the right order
    mov [ebx + eax], dl ;moves the actual byte of the char to the string buffer and ignores the empty dh
    inc eax ;increments eax - counter of chars unstacked
    cmp eax, ecx    ;check with ecx if finished unstacking all chars
    jne unstack_result  ;jump if not finished
    cmp esi, 0  ;checks if negative again
    xor edx, edx    ;resets edx
    je finish_after_check_intchar_hex   ;jumps if positive
    inc edx ;edx will have 1 more byte to print, in case of negative

    finish_after_check_intchar_hex:
    add esp, 4 ;remove the value 16 from top of stack
print_output:
    ;outputs number
    push ecx    ;saves ecx - digits converted
    mov eax, 4
    mov ebx, 1
    add edx, ecx
    mov ecx, esp
    add ecx, 12 ;12 bytes of buffer and 4 bytes of pushed ecx - get to the start of the buffer
    int 80H

    ;outputs a linebreak
    push word 0d0aH ;add linebreak to stack
    mov eax, 4
    mov ebx, 1
    mov ecx, esp
    mov edx, 2
    int 80H
    add esp, 2 ;remove linebreak from stack

    pop eax ;moved ecx into eax - digit counter
    add esp, 8    ;remove buffer from top of stack

    pop edx ;-edx
    pop ecx ;-ecx
    pop ebx ;-ebx

    pop ebp ;removed stack frame
    ret 4 ;returns and removes the argument received from stack


exit:
    ;exit
    mov eax, 1
    mov ebx, 0
    int 80H
