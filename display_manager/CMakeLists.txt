add_library(display_manager "")

target_sources(display_manager
  PRIVATE
    display_manager.cc
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/display_manager.h
)

target_link_libraries(display_manager 
  PUBLIC
    font
    sh1106_spi
)

target_include_directories(display_manager
  PUBLIC
    ${CMAKE_SOURCE_DIR}/
)