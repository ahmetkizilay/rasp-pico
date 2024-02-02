add_executable(macropad_text)

target_sources(macropad_text 
PRIVATE 
    main.cc
)

target_link_libraries(macropad_text 
PRIVATE
    hardware_spi
    adafruit_macropad
    sh1106_spi
    display_manager
    dotmatrix_font
)

pico_add_extra_outputs(macropad_text)