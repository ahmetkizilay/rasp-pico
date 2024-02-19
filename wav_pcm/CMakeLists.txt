set(LIB_NAME "wav_pcm")

add_library(${LIB_NAME} "")

target_sources(${LIB_NAME}
  PRIVATE
    wav_parser.cc
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/wav_parser.h
)

target_include_directories(${LIB_NAME}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/
)
