add_library(font INTERFACE)

target_sources(font
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/font.h
)

target_include_directories(font INTERFACE ${CMAKE_SOURCE_DIR}/)

add_library(dotmatrix_font "")

target_sources(dotmatrix_font
  PRIVATE
    dotmatrix_font.cc
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/dotmatrix_font.h
)

target_link_libraries(dotmatrix_font 
  PUBLIC
    font
)

target_include_directories(dotmatrix_font
  PUBLIC
    ${CMAKE_SOURCE_DIR}/
)