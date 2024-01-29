add_library(sh1106_spi "")

target_sources(sh1106_spi
  PRIVATE
    sh1106_spi.cc
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/sh1106_spi.h
)

target_include_directories(sh1106_spi
  PUBLIC
    ${CMAKE_SOURCE_DIR}/
)

target_link_libraries(sh1106_spi 
  PRIVATE
    pico_stdlib
    hardware_gpio
    hardware_spi
)