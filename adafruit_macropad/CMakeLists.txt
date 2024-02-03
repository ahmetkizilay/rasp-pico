add_library(adafruit_macropad "")

target_sources(adafruit_macropad
  PRIVATE
    adafruit_macropad.cc
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/adafruit_macropad.h
)

target_link_libraries(adafruit_macropad
  PUBLIC
    display_manager
    dotmatrix_font
    sh1106_spi
    hardware_gpio
)

target_include_directories(adafruit_macropad
  PUBLIC
    ${CMAKE_SOURCE_DIR}/
)