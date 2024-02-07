add_executable(macropad_blink
    macropad_blink.cc
)

target_link_libraries(macropad_blink pico_stdlib)

pico_enable_stdio_usb(macropad_blink 1)
pico_enable_stdio_uart(macropad_blink 0)

pico_add_extra_outputs(macropad_blink)

pico_flash(macropad_blink)
