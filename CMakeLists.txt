cmake_minimum_required(VERSION 4.0)
project(index)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(COMPILE_WARNING_AS_ERROR ON)

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
    -sASSERTIONS=1                 # Enable runtime assertions
    -sASYNCIFY=1                   # Required for async WebGPU operations
    -sEXPORTED_FUNCTIONS=['_main'] # Export main function
    -sEXPORTED_RUNTIME_METHODS=['ccall','cwrap']
    -sALLOW_MEMORY_GROWTH=1        # Allow memory growth
    -sINITIAL_MEMORY=67108864      # 64MB initial memory
    -sMAXIMUM_MEMORY=134217728     # 128MB max memory
    -O2                            # Optimize for performance
    --embed-file ${CMAKE_SOURCE_DIR}/src/shaders@/shaders
)