add_executable(macropad_ws2812)

target_sources(macropad_ws2812 
PRIVATE 
    main.cc
)

target_link_libraries(macropad_ws2812 
PRIVATE
    hardware_pio
    adafruit_macropad
    ws2812_pio
)

pico_add_extra_outputs(macropad_ws2812)

