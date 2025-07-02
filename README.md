# Emscripten CMake WebGL Project

A C++ WebGL application using Emscripten, CMake, GLFW, and GLM with a modern Node.js development workflow.

## Prerequisites

### Required Tools
1. [Node.js](https://nodejs.org/en/download)
2. [Emscripten SDK (EMSDK)](https://emscripten.org/docs/getting_started/downloads.html)
3. [vcpkg](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started)
4. [CMake](https://cmake.org/download/)
5. [Ninja build system](https://github.com/ninja-build/ninja)

## Setup Instructions

### 1. Install Required Tools
Follow the installation instructions for the [Required Tools](#required-tools) mentioned above.

### 2. Setup Emscripten SDK
As described by the Emscripten docs
```bash
# Clone the emsdk repository
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Install and activate the latest SDK
emsdk install latest
emsdk activate latest

# Activate environment (run this in the terminal where you'll build)
emsdk_env.bat  # Windows
# or
source ./emsdk_env.sh  # Linux/Mac
```

### 3. Setup vcpkg
As described in the vcpkg installation docs
```bash
# Clone vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg

# Bootstrap vcpkg
.\bootstrap-vcpkg.bat  # Windows
# or
./bootstrap-vcpkg.sh   # Linux/Mac
```

Configure the VCPKG_ROOT environment variable.

```bash
$env:VCPKG_ROOT = "C:\path\to\vcpkg"
$env:PATH = "$env:VCPKG_ROOT;$env:PATH"
```

> [!TIP]
> Setting environment variables in this manner only affects the current terminal session. To make these changes permanent across all sessions, set them through the Windows System Environment Variables panel.

### 4. Install NPM Dependencies
```bash
# In the project directory
npm install
```

## Project Structure

```
├── .vscode/
│   ├── c_cpp_properties.json   # VSCode C++ configurations
├── src/                        # C++ source files -- There will be linter errors before building for first time
│   ├── main.cpp           
│   ├── triangle.cpp       
│   ├── triangle.h
│   ├── shader.cpp         
│   └── shader.h
├── build/                      # CMake build artifacts (auto-generated, git ignored)
├── dist/                       # Web output files (auto-generated, git ignored)
├── CMakeLists.txt              # CMake configuration
├── CMakePresets.json           # CMake presets for Emscripten
└── package.json                # Node.js dependencies and scripts
└── vcpkg.json                  # C++ dependencies (auto-installed)
```

## NPM Scripts

### Development Server
```bash
# Start development server with auto-reload
npm run watch
```

### Manual Serving
```bash
# Just serve the built files (without watching)
npm run serve
```

### Release Build
```bash
# Build for release instead of debug
npm run build:release
```

## Development

### Creating Additional Source Files
Create C++ files in `/src` and update `CMakeLists.txt`
```cmake
add_executable(
    index
    src/main.cpp
    src/shader.cpp
    src/triangle.cpp
    src/myFile.cpp # Add your file here
)
```

### Adding Additional C++ Dependencies
Add dependencies to `vcpkg.json`
```json
{
  "name": "your-project",
  "version": "1.0.0",
  "dependencies": [
    "glfw3",
    "glm",
    "fmt" // Add your dependency here
  ]
}
```

Update `find_package()` and `target_link_libraries()` in `CMakeLists.txt`
```cmake
# Find packages installed by vcpkg
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED) # Add dependency here

# Link libraries
target_link_libraries(index PRIVATE 
    glfw 
    glm::glm
    fmt::fmt # And here
)
```

Dependencies will be automatically installed on next build

## Deployment 

### Setting Up Github Pages
- Go to your repository on GitHub
- Navigate to Settings → Pages
- Under "Source", select GitHub Actions
- GitHub will automatically detect the workflow file

### Push to Main
The project includes a `.github/workflows/deploy.yml` file that builds and deploys from `/src` whenever a push is made to main.