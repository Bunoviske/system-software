section .data


user_integer db 0, 0, 0, 0  ;usado apenas na simulacao da funcao
user_char db 0
user_string db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

section .text

global _start

_start:
    ;--int
    push user_integer
    call LerInteiro
    ; pop ebx
    ; mov [user_integer], ebx
    ;--
    mov ebx, [user_integer]
    push ebx
    call EscreverInteiro

    ;--char
    push user_char
    call LerChar
    ; pop ebx
    ; mov [user_char], ebx
    ;--
    mov ebx, [user_char]
    push ebx
    call EscreverChar

    ;--string
    push user_string
    push dword 10
    call LerString
    ;--
    push user_string
    push dword 10
    call EscreverString

    ;--hex
    push user_integer
    call LerHexa
    ; pop ebx
    ; mov [user_integer], ebx
    ;--
    mov ebx, [user_integer]
    push ebx
    call EscreverHexa

    mov eax, 1
    mov ebx, 0
    int 0X80






LerInteiro:
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

    get_input_charbychar_integer:
    mov eax, 3
    mov ebx, 0  ;0 = stdin - teclado
    mov edx, 1
    int 0X80
    cmp byte [ecx], 0X0A    ;check if char is ENTER
    je finish_input_integer ;if enter - finish input
    mov eax, [ecx]  ;move the char into the string
    mov byte [edi + esi], al
    inc esi ;increment char counter
    cmp esi, 12  ;check if string is full --> counter == string size
    je check_extra_char_enter_integer   ;if yes = string full but still no enter, read and trash next chars until enter
    jmp get_input_charbychar_integer    ;keep getting input

    check_extra_char_enter_integer: ;read char and throw it away until enter
    mov eax, 3
    mov ebx, 0  ;0 = stdin - teclado
    mov edx, 1
    int 0X80
    cmp byte [ecx], 0X0A    ;check if char is ENTER
    jne check_extra_char_enter_integer

    finish_input_integer:   ;finish getting inputs

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
    mov edx, [ebp + 8]
    mov [edx], eax   ;no arguments - return value is ebp + 8 bytes pushed by call and frame creation (return and ebp)
    mov eax, ecx    ;move counter into eax - number of characters input

    pop esi ;pops original value of: esi
    pop edx ; -edx
    pop ecx ; -ecx
    pop ebx ; -ebx

    pop ebp ;removing stack frame
    ret


EscreverInteiro:
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
    je unstack_result_integer   ;if positive, jumps
    mov byte [ebx + eax], '-'   ;adds '-' to start of string
    inc eax ;increment eax - counter of chars added to string
    inc ecx ;increment ecx - counter of digits converted - + the negative char
    unstack_result_integer:
    pop dx  ;pops the chars converted to get the right order
    mov [ebx + eax], dl ;moves the actual byte of the char to the string buffer and ignores the empty dh
    inc eax ;increments eax - counter of chars unstacked
    cmp eax, ecx    ;check with ecx if finished unstacking all chars
    jne unstack_result_integer  ;jump if not finished
    cmp esi, 0  ;checks if negative again
    xor edx, edx    ;resets edx
    je finish_after_check_intchar   ;jumps if positive
    inc edx ;edx will have 1 more byte to print, in case of negative

    finish_after_check_intchar:
    add esp, 4 ;remove the value 10from top of stack
print_output_integer:
    ;outputs number
    push ecx    ;saves ecx - digits converted
    mov eax, 4
    mov ebx, 1
    add edx, ecx
    mov ecx, esp
    add ecx, 16 ;12 bytes of buffer and 4 bytes of pushed ecx - get to the start of the buffer
    int 0X80

    ;outputs a linebreak
    push word 0X0D0A ;add linebreak to stack
    mov eax, 4
    mov ebx, 1
    mov ecx, esp
    mov edx, 2
    int 0X80
    add esp, 2 ;remove linebreak from stack

    pop eax ;moved ecx into eax - digit counter
    add esp, 12    ;remove buffer from top of stack
    pop esi ;unstacking orgiginal values of: -esi
    pop edx ;-edx
    pop ecx ;-ecx
    pop ebx ;-ebx

    pop ebp ;removed stack frame
    ret 4 ;returns and removes the argument received from stack


LerChar:
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
    int 0X80

    mov eax, [ecx]  ;moving the char read to the return address on the stack
    mov ecx, [ebp + 8]  ;||
    mov [ecx], eax  ; ||

    dec ecx ;get to the second byte of memory on the buffer - used to read until enter
    check_extra_char_enter_char:
    mov eax, 3
    mov ebx, 0  ;0 = stdin - teclado
    mov edx, 1
    int 0X80
    cmp byte [ecx], 0X0A    ;check if char is ENTER
    jne check_extra_char_enter_char

    mov eax, 1  ;eax contains the number of elements read - always 1 char

    pop cx ; removes the buffer from top of stack

    pop edx
    pop ecx
    pop ebx

    pop ebp ;remove the stack frame

    ret


EscreverChar:
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
    int 0X80

    ;outputs a linebreak
    push word 0X0D0A ;add linebreak to stack
    mov eax, 4
    mov ebx, 1
    mov ecx, esp
    mov edx, 2
    int 0X80
    add esp, 2 ;remove linebreak from stack

    mov eax, 1    ;number of chars printed - always 1 char
    pop edx
    pop ecx
    pop ebx

    pop ebp ;remove stack frame

    ret 4   ;return and remove argument from stack


LerString:
;receive ascii string input
    push ebp        ;creating stack frame
    mov ebp, esp    ;creating stack frame
    push ebx    ;size of string
    push ecx    ;pointer for string and counter
    push edx    ;size of string and string pointer
    push esi    ;used as counter to get inputs - char by char
    push edi    ;used as pointer to string
    ;ask for input



    push word 0 ;reserve buffer to read the extra chars
    mov ecx, esp    ;move the buffer to ecx
    xor esi, esi    ;reset counter
    mov edi, [ebp + 12] ;pointer to string

    get_input_charbychar_string:
    mov eax, 3
    mov ebx, 0  ;0 = stdin - teclado
    mov edx, 1
    int 0X80
    cmp byte [ecx], 0X0A    ;check if char is ENTER
    je finish_input_string ;if enter - finish input
    mov byte al, [ecx]  ;move the char into the string
    mov byte [edi + esi], al
    inc esi ;increment char counter
    cmp esi, [ebp + 8]  ;check if string is full --> counter == string size
    je check_extra_char_enter_string   ;if yes = string full but still no enter, read and trash next chars until enter
    jmp get_input_charbychar_string    ;keep getting input

    check_extra_char_enter_string: ;read char and throw it away until enter
    mov eax, 3
    mov ebx, 0  ;0 = stdin - teclado
    mov edx, 1
    int 0X80
    cmp byte [ecx], 0X0A    ;check if char is ENTER
    jne check_extra_char_enter_string

    finish_input_string:   ;finish fetting inputs

    add esp, 2  ;remove buffer
    pop edi ;pops edi back
    pop esi ;pops esi back


    mov edx, [ebp + 12] ; move the string pointer to edx
    mov ecx, 0  ;reset counter
    mov ebx, [ebp + 8]  ;move the string size to ebx - avoid segfault if string is max size and has no ENTER
    dec ebx ; dec string size to get the last element index - needed to compare with counter (from 1->x to 0->x-1)
    count_input:
    cmp byte [edx], 0X0A    ;check if char is ENTER
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


EscreverString:
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
    cmp byte [edx], 0X0A    ;check if char is ENTER
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
    int 0X80

    ;outputs a linebreak
    push word 0X0D0A ;add linebreak to stack
    mov eax, 4
    mov ebx, 1
    mov ecx, esp
    mov edx, 2
    int 0X80
    add esp, 2 ;remove linebreak from stack

    pop eax ;moved ecx into eax - char counter
    pop edx
    pop ecx
    pop ebx

    pop ebp ;removed stack frame
    ret 8 ;returns and removes the arguments received from stack

LerHexa:
;receive integer (ascii string) input in hex
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
    get_input_charbychar_hex:
    mov eax, 3
    mov ebx, 0  ;0 = stdin - teclado
    mov edx, 1
    int 0X80
    cmp byte [ecx], 0X0A    ;check if char is ENTER
    je finish_input_hex ;if enter - finish input
    mov eax, [ecx]  ;move the char into the string
    mov byte [edi + esi], al
    inc esi ;increment char counter
    cmp esi, 8  ;check if string is full --> counter == string size
    je check_extra_char_enter_hex   ;if yes = string full but still no enter, read and trash next chars until enter
    jmp get_input_charbychar_hex    ;keep getting input

    check_extra_char_enter_hex: ;read char and throw it away until enter
    mov eax, 3
    mov ebx, 0  ;0 = stdin - teclado
    mov edx, 1
    int 0X80
    cmp byte [ecx], 0X0A    ;check if char is ENTER
    jne check_extra_char_enter_hex

    finish_input_hex:   ;finish getting inputs

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
    mov edx, [ebp + 8]
    mov[edx], eax   ;no arguments - return value is ebp + 8 bytes pushed by call and frame creation (return and ebp)
    mov eax, ecx    ;move counter into eax - number of characters input

    pop edx
    pop ecx
    pop ebx

    pop ebp ;removing stack frame

    ret


EscreverHexa:
;print integer (ascii string) output in hex
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
    unstack_result_hex:
    pop dx  ;pops the chars converted to get the right order
    mov [ebx + eax], dl ;moves the actual byte of the char to the string buffer and ignores the empty dh
    inc eax ;increments eax - counter of chars unstacked
    cmp eax, ecx    ;check with ecx if finished unstacking all chars
    jne unstack_result_hex  ;jump if not finished
    cmp esi, 0  ;checks if negative again
    xor edx, edx    ;resets edx
    je finish_after_check_intchar_hex   ;jumps if positive
    inc edx ;edx will have 1 more byte to print, in case of negative

    finish_after_check_intchar_hex:
    add esp, 4 ;remove the value 16 from top of stack
print_output_hex:
    ;outputs number
    push ecx    ;saves ecx - digits converted
    mov eax, 4
    mov ebx, 1
    add edx, ecx
    mov ecx, esp
    add ecx, 12 ;12 bytes of buffer and 4 bytes of pushed ecx - get to the start of the buffer
    int 0X80

    ;outputs a linebreak
    push word 0X0D0A;add linebreak to stack
    mov eax, 4
    mov ebx, 1
    mov ecx, esp
    mov edx, 2
    int 0X80
    add esp, 2 ;remove linebreak from stack

    pop eax ;moved ecx into eax - digit counter
    add esp, 8    ;remove buffer from top of stack

    pop edx ;-edx
    pop ecx ;-ecx
    pop ebx ;-ebx

    pop ebp ;removed stack frame
    ret 4 ;returns and removes the argument received from stack
