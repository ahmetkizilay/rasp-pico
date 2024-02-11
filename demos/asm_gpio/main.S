@
@ Flash three LEDs connected to Raspberry Pi.
@
@ Based on RP2040 Assembly Language Programming by Stephen Smith

#include "hardware/regs/addressmap.h"
#include "hardware/regs/sio.h"
#include "hardware/regs/io_bank0.h"
#include "hardware/regs/pads_bank0.h"

  .EQU LED_PIN1, 18
  .EQU LED_PIN2, 19
  .EQU LED_PIN3, 20
  .EQU sleep_time, 200

.thumb_func
.global main
      
      
      .align 4

main:
  @ initialize the pins
  MOV R0, #LED_PIN1
  BL gpioinit
  MOV R0, #LED_PIN2
  BL gpioinit
  MOV R0, #LED_PIN3
  BL gpioinit

loop:
  @ turn on the LEDs, sleep and turn off
  MOV R0, #LED_PIN1
  BL gpio_on
  LDR R0, =sleep_time
  BL sleep_ms
  MOV R0, #LED_PIN1
  BL gpio_off

  MOV R0, #LED_PIN2
  BL gpio_on
  LDR R0, =sleep_time
  BL sleep_ms
  MOV R0, #LED_PIN2
  BL gpio_off

  MOV R0, #LED_PIN3
  BL gpio_on
  LDR R0, =sleep_time
  BL sleep_ms
  MOV R0, #LED_PIN3
  BL gpio_off
  
  B loop            @ loop forever
  
gpioinit: 
  @ initialize the GPIO
  MOV R3, #1
  LSL R3, R0
  LDR R2, gpiobase
  STR R3, [R2, #SIO_GPIO_OE_SET_OFFSET]
  STR R3, [R2, #SIO_GPIO_OUT_CLR_OFFSET]
  @ enable i/o for the pin
  LDR R2,  padsbank0
  LSL R3, R0, #2
  ADD R2, R3      @ pin*4 + pads_bank0_base
  MOV R1, #PADS_BANK0_GPIO0_IE_BITS
  LDR R4, setoffset
  ORR R2, R4
  STR R1, [R2, #PADS_BANK0_GPIO0_OFFSET]
@ set the function number to SIO
  LSL R0, #3
  LDR R2, iobank0
  ADD R2, R0
  MOV R1, #IO_BANK0_GPIO3_CTRL_FUNCSEL_VALUE_SIO_3
  STR R1, [R2, #IO_BANK0_GPIO0_CTRL_OFFSET]
  BX LR

gpio_on:
  MOV R3, #1
  LSL R3, R0
  LDR R2, gpiobase
  STR R3, [R2, #SIO_GPIO_OUT_SET_OFFSET]
  BX LR

gpio_off:
  MOV R3, #1
  LSL R3, R0
  LDR R2, gpiobase
  STR R3, [R2, #SIO_GPIO_OUT_CLR_OFFSET]
  BX LR

            .align 4
gpiobase:   .word SIO_BASE
iobank0:    .word IO_BANK0_BASE
padsbank0:  .word PADS_BANK0_BASE
setoffset:  .word REG_ALIAS_SET_BITS
