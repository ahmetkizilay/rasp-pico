add_executable(macropad_keypress)

target_sources(macropad_keypress 
PRIVATE 
    main.cc
)

target_link_libraries(macropad_keypress 
  PRIVATE
    pico_time
    adafruit_macropad
    display_manager
)

pico_add_extra_outputs(macropad_keypress)