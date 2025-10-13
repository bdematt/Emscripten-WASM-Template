#include <iostream>
#include <emscripten/html5.h>
#include <webgpu/webgpu_cpp.h>

// Emscripten exposed function, called during window resize
extern "C" {
    EMSCRIPTEN_KEEPALIVE
    void handleResize() {
        std::cout << "Resizing..." << std::endl;
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}