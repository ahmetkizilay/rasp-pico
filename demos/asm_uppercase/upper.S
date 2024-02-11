@ mplementation of a function that converts a string to uppercase.
@ 
@ Based on RP2040 Assembly Language Programming by Stephen Smith
@ R1 - address of output string
@ R0 - address of input string
@ R4 - original output string for length calculation
@ R5 - current character

.global toupper

toupper:
  PUSH {R4-R5}
  MOV R4, R1

loop: 
  LDRB R5, [R0]
  ADD R0, #1
@ if R5 > 'z' then goto cont
  CMP R5, #'z'
  BGT cont
@ if R5 < 'a' then goto cont
  CMP R5, #'a'
  BLT cont
@ the letter is lowercase, convert to uppercase
  SUB R5, #('a' - 'A')
cont:
  STRB R5, [R1] @ store the character to output
  ADD R1, #1
  CMP R5, #0    @ check for null terminator
  BNE loop      @ lloop until null terminator
  SUB R0, R1, R4
  POP {R4-R5}
  BX LR
