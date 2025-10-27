# Emscripten CMake WebGPU Project

A C++ WebGPU application using Emscripten and CMake with a Node.js development workflow.

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
│   └── extensions.json         # Recommended VSCode extensions
├── .github/
│   └── workflows/
│       └── deploy.yml          # GitHub Actions deployment workflow
├── src/                        # C++ source files
│   ├── main.cpp                # Application entry point
│   ├── ShaderLoader.cpp        # Shader loading utility
│   ├── ShaderLoader.h
│   ├── index.html              # HTML shell for WASM
│   └── shaders/
│       └── shader.wgsl         # WebGPU shader
├── build/                      # CMake build artifacts (auto-generated, git ignored)
├── dist/                       # Web output files (auto-generated, git ignored)
├── CMakeLists.txt              # CMake configuration
├── CMakePresets.json           # CMake presets for Emscripten
├── package.json                # Node.js dependencies and scripts
└── vcpkg.json                  # C++ dependencies (auto-installed)
```

## NPM Scripts

### Development
```bash
# Build debug version and start development server with auto-reload
npm run watch
```

## Development

### Creating Additional Source Files
1. Create C++ files in `/src`
2. Update `CMakeLists.txt` to include them:

```cmake
add_executable(
    index
    src/main.cpp
    src/ShaderLoader.cpp
    src/myNewFile.cpp  # Add your file here
)
```

### Adding C++ Dependencies via vcpkg

This template uses vcpkg for C++ dependency management. Here's how to add libraries:

1. **Add to vcpkg.json:**
```json
{
  "name": "your-project",
  "version": "1.0.0",
  "dependencies": [
    "glfw3",      // For example
  ]
}
```

2. **Update CMakeLists.txt:**
```cmake
# Find packages installed by vcpkg
find_package(glfw3 CONFIG REQUIRED)

# Link libraries to your executable
target_link_libraries(index PRIVATE 
    glfw
)
```

### Adding Shader Files

Shader files in `src/shaders/` are automatically embedded into the WASM binary and accessible at runtime via `/shaders/` path:

```cpp
// Load shader from embedded file
wgpu::ShaderModule shader = ShaderLoader::fromFile(device, "/shaders/myShader.wgsl");
```

To add more shader directories, update the `--embed-file` flag in `CMakeLists.txt`:
```cmake
target_link_options(index PRIVATE
    --embed-file ${CMAKE_SOURCE_DIR}/src/shaders@/shaders
    --embed-file ${CMAKE_SOURCE_DIR}/src/other-assets@/assets  # Add more here
)
```

## Deployment 

### Setting Up GitHub Pages
1. Go to your repository on GitHub
2. Navigate to Settings → Pages
3. Under "Source", select GitHub Actions
4. GitHub will automatically detect the workflow file

### Push to Main
The project includes a `.github/workflows/deploy.yml` file that:
- Builds the release version
- Deploys to GitHub Pages
- Triggers automatically on pushes to `main`

Your app will be available at: `https://yourusername.github.io/your-repo-name/`

## Troubleshooting

### Build fails with "EMSDK not found"
Make sure you've run `source ./emsdk_env.sh` (or `.bat` on Windows) in your current terminal session.

### Build fails with "VCPKG_ROOT not set"
Ensure the `VCPKG_ROOT` environment variable points to your vcpkg installation directory.