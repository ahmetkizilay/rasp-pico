include_directories(${CMAKE_SOURCE_DIR})
add_executable(asm_gpio
    main.S
)

target_link_libraries(asm_gpio pico_stdlib)

pico_add_extra_outputs(asm_gpio)

pico_flash(asm_gpio)
