add_executable(asm_hello
    HelloWorld.S
)

target_link_libraries(asm_hello pico_stdlib)

pico_enable_stdio_usb(asm_hello 1)
pico_enable_stdio_uart(asm_hello 0)

pico_add_extra_outputs(asm_hello)

pico_flash(asm_hello)
