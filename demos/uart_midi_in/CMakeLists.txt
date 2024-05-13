set(CURRENT_TARGET_NAME "uart_midi_in")

add_executable(${CURRENT_TARGET_NAME}
    main.cc
)

target_link_libraries(${CURRENT_TARGET_NAME} 
  pico_stdlib
  hardware_timer
  hardware_uart
)

pico_enable_stdio_usb(${CURRENT_TARGET_NAME} 1)
pico_enable_stdio_uart(${CURRENT_TARGET_NAME} 0)

pico_add_extra_outputs(${CURRENT_TARGET_NAME})

pico_flash(${CURRENT_TARGET_NAME})
