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
    call INPUT

    pop ebx
    mov [user_integer], ebx
    ;-----
    mov eax, [user_integer]
    push eax    ;pushing argument
    call OUTPUT
    jmp exit

INPUT:
;receive integer (ascii string) input
    push ebp        ;creating stack frame
    mov ebp, esp    ;creating stack frame
    push ebx    ;dont push eax because eax is the return value
    push ecx    ;counter of elements
    push edx    ;pointer for char string
    push esi    ;flag for negative

    push dword 0    ;reserve space to use as buffer
    push dword 0
    push dword 0
    ;ask for input

    push edi
    push word 0 ;reserve buffer to read the extra chars
    mov ecx, esp    ;move the buffer to ecx
    xor esi, esi    ;reset counter
    mov edi, esp ;pointer to string
    add edi, 18

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
    cmp esi, 12  ;check if string is full --> counter == string size
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
    add edx, 12
    push dword 10 ;stack the value 10 to multiply
    xor esi, esi  ;zero the negative number flag
    xor ecx, ecx ; zero the counter


convert_charint:
    cmp ecx, 11 ;avoids acessing an extra element
    je finish_charint
    movzx ebx, byte [edx] ;get char
    inc edx ;prepare next char
    inc ecx ;increment counter
    cmp ebx, '-' ;check for negative
    je negative_charint ;set negative flag

    cmp ebx, '0'    ;check if integer
    jb finish_charint
    cmp ebx, '9'    ;check if integer
    ja finish_charint
    sub ebx, '0' ;convert to integer
    push edx    ;save edx before ultiplication
    mul dword [esp+4] ;multiply eax by 10 = get ready for next digit
    pop edx ;no use for edx, maximum value of multiplication fits eax
    add eax, ebx    ;adds toacumulator value
    jmp convert_charint ;jump until finished
negative_charint:
    mov esi, 1  ;esi is a flag or negative
    jmp convert_charint

finish_charint:
    dec ecx ; counter will have 1 extra from the ending char
    add esp, 16  ;remove 10 and buffer from top of stack
    cmp esi, 1 ;check if negative
    jne finish_after_check_charint  ;if not negative, go on to finish
    imul eax, -1    ;if negative, multiply by -1
finish_after_check_charint:
    mov[ebp + 8], eax   ;no arguments - return value is ebp + 8 bytes pushed by call and frame creation (return and ebp)
    mov eax, ecx    ;move counter into eax - number of characters input

    pop esi ;pops original value of: esi
    pop edx ; -edx
    pop ecx ; -ecx
    pop ebx ; -ebx

    pop ebp ;removing stack frame
    ret


OUTPUT:
;print integer (ascii string) output
    push ebp    ;creating stack frame
    mov ebp, esp    ;creating stack frame
    push ebx    ;dont push eax because eax is the return value
    push ecx    ;digit counter
    push edx    ;rest of division - number to convert
    push esi    ;flag for negative

    push dword 0    ;reserve space to use as buffer
    push dword 0
    push dword 0



    mov eax, [ebp + 8]   ;number - passed as argument through stack
    mov ebx, esp   ;string
    add ebx, 12
    mov ecx, 0  ;digit counter
    push dword 10 ;stack the value 10 to divide

check_negative:
    xor esi, esi    ;set negative flag to zero
    cmp eax, 0  ;check if number is smaller then 0
    jge convert_intchar ;jump if number is positive
    mov edx, -1 ;put -1 on edx for multiplication
    imul edx     ;if negative multiply number by -1 to operate on unsigned number
    mov esi, 1 ; esi is a negative flag
convert_intchar:
    xor edx, edx    ;zero edx (rest of division - number to convert)
    div dword [esp + ecx * 2] ;divide by 10
    add edx, '0'    ;convert digit to ascii char
    push dx ;stack number converted to ascii - stack works with a minimum 16 bits
    inc ecx ;count number of digits converted
    cmp eax, 0  ;check if eax is 0 or if there is still stuff to divide
    jne convert_intchar ;jumps while not finished



output_intchar:
    mov eax, 0  ;reset eax to use it as counter of chars added to string
    cmp esi, 0  ;check if negative
    je unstack_result   ;if positive, jumps
    mov byte [ebx + eax], '-'   ;adds '-' to start of string
    inc eax ;increment eax - counter of chars added to string
    inc ecx ;increment ecx - counter of digits converted - + the negative char
    unstack_result:
    pop dx  ;pops the chars converted to get the right order
    mov [ebx + eax], dl ;moves the actual byte of the char to the string buffer and ignores the empty dh
    inc eax ;increments eax - counter of chars unstacked
    cmp eax, ecx    ;check with ecx if finished unstacking all chars
    jne unstack_result  ;jump if not finished
    cmp esi, 0  ;checks if negative again
    xor edx, edx    ;resets edx
    je finish_after_check_intchar   ;jumps if positive
    inc edx ;edx will have 1 more byte to print, in case of negative

    finish_after_check_intchar:
    add esp, 4 ;remove the value 10from top of stack
print_output:
    ;outputs number
    push ecx    ;saves ecx - digits converted
    mov eax, 4
    mov ebx, 1
    add edx, ecx
    mov ecx, esp
    add ecx, 16 ;12 bytes of buffer and 4 bytes of pushed ecx - get to the start of the buffer
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
    add esp, 12    ;remove buffer from top of stack
    pop esi ;unstacking orgiginal values of: -esi
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
