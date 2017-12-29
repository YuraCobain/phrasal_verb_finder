set(target_name logger)

set(target_src src/logger.cpp)

include_directories(inc)
add_library(${target_name} STATIC ${target_src})
target_include_directories(${target_name} INTERFACE inc)

set_target_properties(${target_name} PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

