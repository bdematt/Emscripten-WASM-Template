#include "ShaderLoader.h"
#include <dawn/webgpu_cpp_print.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <iostream>
#include <webgpu/webgpu_cpp.h>

// Canvas & Window
uint32_t width = 512;
uint32_t height = 512;

// WGPU
wgpu::Instance instance;
wgpu::Adapter adapter;
wgpu::Device device;
wgpu::Surface surface;
wgpu::TextureFormat format;
wgpu::RenderPipeline pipeline;

void createRenderPipeline() {
    wgpu::ShaderModule shaderModule = ShaderLoader::fromFile(device, "/shaders/shader.wgsl");
    wgpu::ColorTargetState colorTargetState{.format = format};
    wgpu::FragmentState fragmentState{
        .module = shaderModule, .targetCount = 1, .targets = &colorTargetState};

    wgpu::RenderPipelineDescriptor descriptor{.vertex = {.module = shaderModule},
                                              .fragment = &fragmentState};
    pipeline = device.CreateRenderPipeline(&descriptor);
}

void setCanvasDimensions() {
    int iWidth, iHeight;
    emscripten_get_canvas_element_size("#canvas", &iWidth, &iHeight);
    width = static_cast<uint32_t>(iWidth);
    height = static_cast<uint32_t>(iHeight);
}

void configureSurface() {
    wgpu::SurfaceCapabilities capabilities;
    surface.GetCapabilities(adapter, &capabilities);
    format = capabilities.formats[0];

    wgpu::SurfaceConfiguration config{.device = device,
                                      .format = format,
                                      .width = width,
                                      .height = height,
                                      .presentMode = wgpu::PresentMode::Fifo};
    surface.Configure(&config);
}

void initGraphics() {
    configureSurface();
    createRenderPipeline();
}

void render() {
    wgpu::SurfaceTexture surfaceTexture;
    surface.GetCurrentTexture(&surfaceTexture);

    wgpu::RenderPassColorAttachment attachment{.view = surfaceTexture.texture.CreateView(),
                                               .loadOp = wgpu::LoadOp::Clear,
                                               .storeOp = wgpu::StoreOp::Store};

    wgpu::RenderPassDescriptor renderpass{.colorAttachmentCount = 1,
                                          .colorAttachments = &attachment};

    wgpu::CommandEncoder encoder = device.CreateCommandEncoder();
    wgpu::RenderPassEncoder pass = encoder.BeginRenderPass(&renderpass);
    pass.SetPipeline(pipeline);
    pass.Draw(3);
    pass.End();
    wgpu::CommandBuffer commands = encoder.Finish();
    device.GetQueue().Submit(1, &commands);
}

void init() {
    // Canvas Dimensions
    setCanvasDimensions();

    // Create Instance
    static const auto kTimedWaitAny = wgpu::InstanceFeatureName::TimedWaitAny;
    wgpu::InstanceDescriptor instanceDesc{.requiredFeatureCount = 1,
                                          .requiredFeatures = &kTimedWaitAny};
    instance = wgpu::CreateInstance(&instanceDesc);

    // Request Adapter
    wgpu::Future f1 = instance.RequestAdapter(
        nullptr, wgpu::CallbackMode::WaitAnyOnly,
        [](wgpu::RequestAdapterStatus status, wgpu::Adapter a, wgpu::StringView message) {
            if (status != wgpu::RequestAdapterStatus::Success) {
                std::cout << "RequestAdapter: " << message << "\n";
                exit(0);
            }
            adapter = std::move(a);
        });
    instance.WaitAny(f1, UINT64_MAX);

    // Request Device
    wgpu::DeviceDescriptor desc{};
    desc.SetUncapturedErrorCallback(
        [](const wgpu::Device&, wgpu::ErrorType errorType, wgpu::StringView message) {
            std::cout << "Error: " << errorType << " - message: " << message << "\n";
        });
    wgpu::Future f2 = adapter.RequestDevice(
        &desc, wgpu::CallbackMode::WaitAnyOnly,
        [](wgpu::RequestDeviceStatus status, wgpu::Device d, wgpu::StringView message) {
            if (status != wgpu::RequestDeviceStatus::Success) {
                std::cout << "RequestDevice: " << message << "\n";
                exit(0);
            }
            device = std::move(d);
        });
    instance.WaitAny(f2, UINT64_MAX);

    // Create Surface from Canvas
    wgpu::EmscriptenSurfaceSourceCanvasHTMLSelector canvasDesc{};
    canvasDesc.selector = "#canvas";

    wgpu::SurfaceDescriptor surfaceDesc{};
    surfaceDesc.nextInChain = &canvasDesc;
    surface = instance.CreateSurface(&surfaceDesc);

    initGraphics();
}

void mainLoop() {
    render();
    instance.ProcessEvents();
}

int main() {
    init();
    emscripten_set_main_loop(mainLoop, 0, true);
    return 0;
}