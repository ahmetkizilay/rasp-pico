# macro to flash the binary source file to the pico.
# this macro can only be used with executables, otherwise it will throw an error.
macro(pico_flash TARGET)
get_target_property(target_type ${TARGET} TYPE)
if (NOT target_type STREQUAL "EXECUTABLE")
  message(FATAL_ERROR "${TARGET} is not executable!")
endif ()
add_custom_target(${TARGET}_flash
  COMMAND sudo picotool info
  COMMAND sudo picotool load $<TARGET_FILE_DIR:${TARGET}>/${TARGET}.uf2
  COMMAND sudo picotool reboot
  DEPENDS ${TARGET}
)
endmacro()

# macro to flash the binary and data.bin file to the pico
# it is required that the data directory is in the same directory as the CMakeLists.txt file
# it is assumed that the data directory only contains files that are to be concatenated 
# into a single binary file to be flashed to the pico in addition to the binary file.
# this macro can only be used with executables, otherwise it will throw an error.
macro(pico_flash_with_data TARGET)
get_target_property(target_type ${TARGET} TYPE)
if (NOT target_type STREQUAL "EXECUTABLE")
  message(FATAL_ERROR "${TARGET} is not executable!")
endif ()

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/data)
  message(FATAL_ERROR "No data directory found in ${CMAKE_CURRENT_SOURCE_DIR}")
endif ()


file(GENERATE OUTPUT ${TARGET}_generate_data_include
  INPUT ${CMAKE_SOURCE_DIR}/cmake/generate_data_include.sh
)

add_custom_target(${TARGET}_gen_data
  COMMAND echo "Generating data include file..."
  COMMAND rm -rf ${CMAKE_CURRENT_BINARY_DIR}/generated
  COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/generated
  COMMAND sh $<TARGET_FILE_DIR:${TARGET}>/${TARGET}_generate_data_include ${CMAKE_CURRENT_SOURCE_DIR}/data/

  # create a custom command to generate the data.bin file
  # this will concatenate all the files in the data directory into a single binary file
  COMMAND echo "Generating data.bin..."
  COMMAND touch data.bin && cp /dev/null data.bin
  COMMAND ls -1 ${CMAKE_CURRENT_SOURCE_DIR}/data/* | xargs cat >> data.bin
)

# generate a script to flash the binary as well as the data.bin file to the pico
file(GENERATE OUTPUT ${TARGET}_flash_with_data
  CONTENT "
#!/bin/bash
set -e

binary_size=$(stat -c %s $<TARGET_FILE_DIR:${TARGET}>/${TARGET}.bin)
printf \"Binary Size: 0x%x\n\" \$binary_size

data_offset=$((($binary_size + 0x1000) - ($binary_size % 0x1000)))
printf \"Data Offset: 0x%x\n\" \$data_offset

flash_location=$((0x10000000 + $data_offset))
flash_location_hex=$(printf 0x%X \$flash_location)
echo \"Flash location: \$flash_location_hex\"

sudo picotool info
echo \"Flashing ${TARGET}\"
sudo picotool load $<TARGET_FILE_DIR:${TARGET}>/${TARGET}.uf2
echo \"Flashing data for ${TARGET}\"
sudo picotool load -v $<TARGET_FILE_DIR:${TARGET}>/data.bin -o $flash_location_hex 
echo \"Verifying ${TARGET}\"
sudo picotool verify $<TARGET_FILE_DIR:${TARGET}>/${TARGET}.uf2
echo \"Rebooting\"
sudo picotool reboot
"
)

# target to flash the binary and data.bin file to the pico
add_custom_target(${TARGET}_flash_with_data
  command sh $<TARGET_FILE_DIR:${TARGET}>/${TARGET}_flash_with_data
  DEPENDS ${TARGET}
)
endmacro()
