#!/bin/bash
set -e

data_dir=$1

echo "
#ifndef EXTRA_DATA_H
#define EXTRA_DATA_H

#include <map>
#include <string>

extern char __flash_binary_end;

namespace crynsnd {
namespace {
  const static std::map<std::string, std::pair<uint32_t, uint32_t>> files{" >> generated/extra_data.h

offset=0x0
for file in $(ls -1 "$data_dir"*); do
  file_name=$(basename $file)
  file_size=$(stat -c %s $file)
  file_size=$(printf "0x%X" $file_size)
  echo "    {\"$file_name\", {$file_size, $offset}}," >> generated/extra_data.h
  offset=$((offset + file_size))
done


echo "  
  };
}

struct DataDescriptor {
  const uint32_t size;
  const uintptr_t address;
};

const DataDescriptor GetDataDescriptor(const std::string& file_name) {
  const auto it = files.find(file_name);
  if (it == files.end()) {
    return DataDescriptor{0, 0};
  }
  const uint32_t file_size = it->second.first;
  const uintptr_t file_offset = it->second.second;

  // This is the address where the program binary ends in flash.
  // The data is stored after the program binary.
  // We use the next 4k aligned address after the end of the binary.
  uintptr_t binary_end = (uintptr_t)&__flash_binary_end;
  const uintptr_t flash_offset = ((binary_end >> 12) + 1) << 12;
  
  const DataDescriptor data = {
    .size = file_size,
    .address = flash_offset + file_offset,
  };
  return data;
}

}  // namespace crynsnd
#endif" >> generated/extra_data.h
