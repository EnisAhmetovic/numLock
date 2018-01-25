%include "asm_io.inc"
section .data
 v005 DD 0
 v001 DD 0

section .text
 global _start
_start:
 MOV CX,5
LOOPSTART1: 
 PUSH CX
 POP CX
 DEC CX
 JNZ LOOPSTART1
INT 80h
MOV EAX,1
MOV EBX,0
INT 80h;