cmake_minimum_required(VERSION 4.0)
project(index)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find packages installed by vcpkg
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Your executable
add_executable(
    index
    src/main.cpp
    src/shader.cpp
    src/triangle.cpp
)

# Link libraries
target_link_libraries(index PRIVATE 
    glfw 
    glm::glm
)

# Create dist directory for web assets
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/dist)

# Emscripten-specific settings
set_target_properties(index PROPERTIES
    SUFFIX ".html"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist
)

target_link_options(index PRIVATE
    -sUSE_WEBGL2=1
    -sUSE_GLFW=3
    -sFULL_ES3=1
    -sEXPORTED_FUNCTIONS=['_main']
    -sEXPORTED_RUNTIME_METHODS=['ccall','cwrap']
)