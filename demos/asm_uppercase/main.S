@ File: main.S
@ Description: This assembly file converts a string to all uppercase.
@ Based on RP2040 Assembly Language Programming by Stephen Smith

.thumb_func
.global main

main: BL stdio_init_all
repeat:
  LDR R0, =instr
  LDR R1, =outstr
  MOV R4, #12
  MOV R5, #13
  BL toupper
  LDR R0, =outstr
  BL printf
  B repeat
.data
instr: .asciz "Hello, World!\n"
outstr: .fill 255, 1, 0
