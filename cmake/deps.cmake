include(FetchContent)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

# IMPORTANT: After changing this value make sure to clean CMake Cache and rebuild.
# See README for explicit commands
set(PICO_BOARD pico CACHE STRING "Board type")
# set(PICO_BOARD pico2 CACHE STRING "Board type")


include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
