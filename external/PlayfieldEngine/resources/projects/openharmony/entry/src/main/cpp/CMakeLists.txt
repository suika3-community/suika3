# the minimum version of CMake.
cmake_minimum_required(VERSION 3.5.0)
project(MyApp1)

set(NATIVERENDER_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

if(DEFINED PACKAGE_FIND_FILE)
    include(${PACKAGE_FIND_FILE})
endif()

include_directories(${NATIVERENDER_ROOT_PATH}
                    ${NATIVERENDER_ROOT_PATH}/include)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Playfield REQUIRED)
target_link_libraries(entry PUBLIC Playfield::Playfield)

add_library(engine SHARED napi_init.cpp)
target_link_libraries(
  engine
  PUBLIC
  Playfield::Playfield
  libace_napi.z.so
  libace_ndk.z.so
  librawfile.z.so
  libhilog_ndk.z.so
  EGL
  GLESv3
  OpenSLES
)
