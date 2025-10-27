cmake_minimum_required(VERSION 4.0)
project(index)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

# Your executable
add_executable(
    index
    src/main.cpp
    src/ShaderLoader.cpp
)

# Create dist directory for web assets
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/dist)

# Emscripten-specific settings
set_target_properties(index PROPERTIES
    SUFFIX ".html"
    LINK_FLAGS "-s WASM=1 --shell-file ${CMAKE_SOURCE_DIR}/src/index.html"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist
)

# Emscripten compile options
target_compile_options(index PRIVATE
    --use-port=emdawnwebgpu
)

# Emscripten link options
target_link_options(index PRIVATE
    --use-port=emdawnwebgpu
    -sASSERTIONS=1
    -sASYNCIFY=1
    -sEXPORTED_FUNCTIONS=['_main','_handleResize']  # Export the resize handler
    -sEXPORTED_RUNTIME_METHODS=['ccall','cwrap']
    -sALLOW_MEMORY_GROWTH=1
    -sINITIAL_MEMORY=67108864
    -sMAXIMUM_MEMORY=134217728
    --embed-file ${CMAKE_SOURCE_DIR}/src/shaders@/shaders
)

# Add optimization flags based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_options(index PRIVATE
        -O3                         # Max optimization for release
        --closure 1                 # Enable Closure Compiler
    )
else()
    target_link_options(index PRIVATE
        -O0                         # No optimization for debug
        -g                          # Include debug symbols
        -sASSERTIONS=2              # More verbose assertions in debug
    )
endif()