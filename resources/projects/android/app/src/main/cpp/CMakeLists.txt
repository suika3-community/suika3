cmake_minimum_required(VERSION 3.22)

project("suika3")

# Find the Suika3 package from the AAR file.
find_package(suika3 REQUIRED CONFIG)

# Define the libstrato.so
add_library(
  strato
  SHARED
  library.c  # AOT stub
)

# Link libsuika3.a in the AAR to libstrato.so
target_link_libraries(
  strato
  suika3::strato
  android
  log
  m
  GLESv3
  OpenSLES
  log
)

# Force load all symbols fro libsuika3.a
target_link_options(
  strato
  PRIVATE
  "LINKER:--whole-archive"
  suika3::strato
  "LINKER:--no-whole-archive"
)
