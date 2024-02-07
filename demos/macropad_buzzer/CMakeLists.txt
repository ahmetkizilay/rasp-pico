add_executable(macropad_buzzer
    main.cc
)

target_link_libraries(macropad_buzzer
  pico_stdlib
  hardware_pwm
)

pico_enable_stdio_usb(macropad_buzzer 1)
pico_enable_stdio_uart(macropad_buzzer 0)

pico_add_extra_outputs(macropad_buzzer)

pico_flash(macropad_buzzer)
