add_executable(macropad_sh1106)

target_sources(macropad_sh1106 
PRIVATE 
    main.cc
)

target_link_libraries(macropad_sh1106 
PRIVATE
    pico_stdlib
    hardware_spi
    adafruit_macropad
    sh1106_spi
)

pico_enable_stdio_usb(macropad_sh1106 1)
pico_enable_stdio_uart(macropad_sh1106 0)

pico_add_extra_outputs(macropad_sh1106)