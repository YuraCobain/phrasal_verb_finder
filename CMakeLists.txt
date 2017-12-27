cmake_minimum_required(VERSION 3.0)
project(sdl2_lesson_1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(cxxopts)
set(app_bin fresal_verb_finder)

add_executable(${app_bin} main.cpp)
target_link_libraries(${app_bin} cxxopts)

set_target_properties(${app_bin} PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

