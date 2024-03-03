set(LIB_NAME "wav_player")

add_library(${LIB_NAME} "")

target_sources(${LIB_NAME}
  PRIVATE
    wav_parser.cc
    wav_player_pwm.cc
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/wav_parser.h
    ${CMAKE_CURRENT_LIST_DIR}/wav_player_pwm.h
)

target_link_libraries(${LIB_NAME} 
  PUBLIC
    hardware_gpio
    hardware_pwm
)

target_include_directories(${LIB_NAME}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/
)
