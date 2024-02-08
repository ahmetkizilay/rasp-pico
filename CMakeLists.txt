cmake_minimum_required(VERSION 3.25)

# Pull in SDK (must be before project)
set(PICO_SDK_FETCH_FROM_GIT true)
set(PICO_SDK_FETCH_FROM_GIT_TAG "1.5.1")
include(pico_sdk_import.cmake)


project(pico_projects C CXX ASM)
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)


if (PICO_SDK_VERSION_STRING VERSION_LESS "1.5.1")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.5.1 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()

macro(pico_flash TARGET)
get_target_property(target_type ${TARGET} TYPE)
if (NOT target_type STREQUAL "EXECUTABLE")
  message(FATAL_ERROR "${TARGET} is not executable!")
endif ()
add_custom_target(${TARGET}_flash
  COMMAND sudo picotool info
  COMMAND sudo picotool load $<TARGET_FILE_DIR:${TARGET}>/${TARGET}.uf2 && sudo picotool reboot
  DEPENDS ${TARGET}
)
endmacro()

add_subdirectory(adafruit_macropad)
add_subdirectory(demos)
add_subdirectory(display_manager)
add_subdirectory(font)
add_subdirectory(sh1106_spi)
add_subdirectory(ws2812_pio)
