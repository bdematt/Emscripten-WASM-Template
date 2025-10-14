#pragma once

#include <webgpu/webgpu_cpp.h>

#include <string>

class ShaderLoader {
   public:
    static wgpu::ShaderModule fromFile(const wgpu::Device& device, const std::string& filepath);
    static wgpu::ShaderModule createFromCode(const wgpu::Device& device,
                                             const std::string& wgslCode);

   private:
    static std::string loadShaderCode(const std::string& filepath);
};