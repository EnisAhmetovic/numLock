section .data
 v005 DD 0
 v001 DD 0

%include "asm_io.inc"
section .text
 global _start
_start:
 MOV EAX, [v005]
 PUSH EAX
 MOV EAX, [v005]
 ADD EAX,42
 POP EBX
 ADD EAX,EBX
 MOV [v005],EAX
INT 80h
MOV EAX,1
MOV EBX,0
INT 80h;