set(target_name phrasal_engine)

set(target_src src/hash_dictionary.cpp
			   src/phrase_pattern.cpp
			   src/phrasal_engine.cpp)

include_directories(inc)

add_library(${target_name} STATIC ${target_src})

target_include_directories(${target_name} INTERFACE inc)

target_link_libraries(${target_name} logger)

set_target_properties(${target_name} PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

