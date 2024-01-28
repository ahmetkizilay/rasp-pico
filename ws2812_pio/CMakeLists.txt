add_library(ws2812_pio "")

target_sources(ws2812_pio
  PRIVATE
    ws2812_pio.cc
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/ws2812_pio.h
)

target_include_directories(ws2812_pio
  PUBLIC
    ${CMAKE_SOURCE_DIR}/
)

target_link_libraries(ws2812_pio 
  PRIVATE
    pico_stdlib
    hardware_pio
)

pico_generate_pio_header(ws2812_pio ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)