l1:
equ 1
L2:
 EQU 0

;;;;;;;;;;;;;;;;  TEXTO  ;;;;;;;;;;;;

section text

;;;;;;;;;;;;;;;;  INSTRUCOES ;;;;;;;;;;;;
IF L1
INPUT l6 + 5
IF L2
OUTPUT l6 + 9

OLA:
LOAD L5 + 1
ADD L4
SUB L4
MUL L4
DIV L4
; JMP OLA
; JMPP OLA
; JMPN OLA
; JMPZ OLA
STORE L8
COPY L7, L6

INPUT L8 + 1
OUTPUT L8 + 1

C_INPUT L8 + 1
C_OUTPUT L8 + 1

H_INPUT L8 + 1
H_OUTPUT L8 + 1

S_INPUT L8, L5
S_OUTPUT L8, L5


STOP

;;;;;;;;;;;;;;;;  DADOS ;;;;;;;;;;;;

section DATA
L4:
CONST 2

L5:
 CONST 0X10F
L6: SPACE 0X10
L7: CONST 0
L8: SPACE 4
