%include "asm_io.inc"
section .data
 v005 DD 0

section .text
 global _start
_start:
 MOV EAX,[v005]
 CALL print_int
 MOV EAX,1
 CALL print_int
INT 80h
MOV EAX,1
MOV EBX,0
INT 80h;