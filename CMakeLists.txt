cmake_minimum_required(VERSION 3.25)

include(cmake/deps.cmake)
include(cmake/pico_flash_utils.cmake)

project(pico_projects C CXX ASM)
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

# Initialize the pico_sdk
pico_sdk_init()

add_subdirectory(adafruit_macropad)
add_subdirectory(demos)
add_subdirectory(display_manager)
add_subdirectory(font)
add_subdirectory(sh1106_spi)
add_subdirectory(wav_player)
add_subdirectory(ws2812_pio)
