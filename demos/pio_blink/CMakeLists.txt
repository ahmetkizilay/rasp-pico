add_executable(pio_blink)

pico_generate_pio_header(pio_blink ${CMAKE_CURRENT_LIST_DIR}/hello.pio)

target_sources(pio_blink 
PRIVATE 
    hello.cc
)

target_link_libraries(pio_blink 
PRIVATE
    pico_stdlib
    hardware_pio
)

pico_add_extra_outputs(pio_blink)

pico_flash(pio_blink)
