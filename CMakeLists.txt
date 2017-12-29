cmake_minimum_required(VERSION 3.0)
project(phrasal_verb)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

add_subdirectory(cxxopts)
add_subdirectory(logger)
add_subdirectory(phrasal_engine)

set(app_bin phrasal_verb_finder)

add_executable(${app_bin} main.cpp)
target_link_libraries(${app_bin} cxxopts logger phrasal_engine)

set_target_properties(${app_bin} PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

