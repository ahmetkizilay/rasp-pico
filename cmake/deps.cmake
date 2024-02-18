include(FetchContent)

FetchContent_Declare(
  pico_sdk
  GIT_REPOSITORY https://github.com/raspberrypi/pico-sdk
  GIT_TAG "1.5.1"
  GIT_SUBMODULES_RECURSE FALSE)

if (NOT pico_sdk)
  message("Downloading Raspberry Pi Pico SDK")
  FetchContent_Populate(pico_sdk)
  set(PICO_SDK_PATH ${pico_sdk_SOURCE_DIR})
  include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
endif ()
