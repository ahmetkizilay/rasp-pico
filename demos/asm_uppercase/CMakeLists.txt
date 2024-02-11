add_executable(asm_uppercase
    main.S
    upper.S
)

target_link_libraries(asm_uppercase
    pico_stdlib
)

# usb enabled, uart disabled
pico_enable_stdio_usb(asm_uppercase 1)
pico_enable_stdio_uart(asm_uppercase 0)

pico_add_extra_outputs(asm_uppercase)

pico_flash(asm_uppercase)