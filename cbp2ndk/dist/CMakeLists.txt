cmake_minimum_required(VERSION 2.6)
project(cbp2ndk)

file(GLOB_RECURSE sources  ../src/*.cpp ../src/*.h ../src/extern/*.h)
file(GLOB_RECURSE data ../src/*.rc)
set(CMAKE_BUILD_TYPE "Release")

add_executable(cbp2ndk ${sources} ${data})
target_compile_options(cbp2ndk PUBLIC -std=c++17 -Wall -Wfloat-conversion)
target_include_directories(cbp2ndk PUBLIC ../src ../src/extern)
