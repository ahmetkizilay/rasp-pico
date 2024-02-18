add_executable(macropad_flash
    main.cc
)

target_link_libraries(macropad_flash pico_stdlib adafruit_macropad)

pico_enable_stdio_usb(macropad_flash 1)
pico_enable_stdio_uart(macropad_flash 0)

pico_add_extra_outputs(macropad_flash)

pico_flash(macropad_flash)
