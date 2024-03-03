set(CURRENT_TARGET_NAME "wav_demo")

add_executable(${CURRENT_TARGET_NAME}
    main.cc
)

add_dependencies(${CURRENT_TARGET_NAME} ${CURRENT_TARGET_NAME}_gen_data)

target_include_directories(${CURRENT_TARGET_NAME} PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/generated
)
target_link_libraries(${CURRENT_TARGET_NAME} 
  pico_stdlib
  adafruit_macropad 
  wav_pcm
)

pico_enable_stdio_usb(${CURRENT_TARGET_NAME} 1)
pico_enable_stdio_uart(${CURRENT_TARGET_NAME} 0)

pico_add_extra_outputs(${CURRENT_TARGET_NAME})

pico_flash_with_data(${CURRENT_TARGET_NAME})
