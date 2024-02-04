@
@ Print out "hello world"
@
@ R0 - first param to printf
@ R1 - second param to printd
@ R7 - index counter
@ 

.thumb_func
.global main

main:
  MOV R7, #0            @ counter initialized to 0
  BL stdio_init_all     @ calling pico sdk to init usb

loop:
  LDR R0, =helloworld   @ load the address of string
  ADD R7, #1            @ increment the counter
  MOV R1, R7            @ set the counter value to the second arg
  BL printf
  B loop                @ back to the loop

.data
            .align 4
helloworld: .asciz  "Hello World %d\n"
