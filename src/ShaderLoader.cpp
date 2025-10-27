#include "ShaderLoader.h"

#include <dawn/webgpu_cpp_print.h>

#include <fstream>
#include <iostream>
#include <sstream>

std::string ShaderLoader::loadShaderCode(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << filepath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

wgpu::ShaderModule ShaderLoader::fromFile(const wgpu::Device& device, const std::string& filepath) {
    std::string code = loadShaderCode(filepath);
    return createFromCode(device, code);
}

wgpu::ShaderModule ShaderLoader::createFromCode(const wgpu::Device& device,
                                                const std::string& wgslCode) {
    wgpu::ShaderSourceWGSL wgsl{{.code = wgslCode.c_str()}};
    wgpu::ShaderModuleDescriptor shaderModuleDescriptor{.nextInChain = &wgsl};
    return device.CreateShaderModule(&shaderModuleDescriptor);
}