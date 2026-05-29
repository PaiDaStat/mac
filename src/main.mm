#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA

#include "app.h"
#include "gui/main_window.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_metal.h>

#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main() {
    @autoreleasepool {
        glfwSetErrorCallback(glfw_error_callback);

        if (!glfwInit()) {
            return -1;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        GLFWwindow* window = glfwCreateWindow(1600, 900, "MacUSBMon", nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            return -1;
        }

        id<MTLDevice> device = MTLCreateSystemDefaultDevice();
        if (!device) {
            glfwDestroyWindow(window);
            glfwTerminate();
            return -1;
        }

        id<MTLCommandQueue> command_queue = [device newCommandQueue];

        NSWindow* ns_window = glfwGetCocoaWindow(window);
        CAMetalLayer* layer = [CAMetalLayer layer];
        layer.device = device;
        layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        layer.framebufferOnly = YES;

        ns_window.contentView.layer = layer;
        ns_window.contentView.wantsLayer = YES;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOther(window, true);
        ImGui_ImplMetal_Init(device);

        App app;
        app.initialize();

        while (!glfwWindowShouldClose(window)) {
            @autoreleasepool {
                glfwPollEvents();
                app.update();

                int width = 0;
                int height = 0;
                glfwGetFramebufferSize(window, &width, &height);
                layer.drawableSize = CGSizeMake(width, height);

                id<CAMetalDrawable> drawable = [layer nextDrawable];
                if (!drawable) {
                    continue;
                }

                MTLRenderPassDescriptor* render_pass = [MTLRenderPassDescriptor renderPassDescriptor];
                render_pass.colorAttachments[0].texture = drawable.texture;
                render_pass.colorAttachments[0].loadAction = MTLLoadActionClear;
                render_pass.colorAttachments[0].storeAction = MTLStoreActionStore;
                render_pass.colorAttachments[0].clearColor = MTLClearColorMake(0.08, 0.08, 0.09, 1.0);

                ImGui_ImplMetal_NewFrame(render_pass);
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                gui::draw_main_window(app);

                ImGui::Render();

                id<MTLCommandBuffer> command_buffer = [command_queue commandBuffer];
                id<MTLRenderCommandEncoder> encoder =
                    [command_buffer renderCommandEncoderWithDescriptor:render_pass];
                ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), command_buffer, encoder);
                [encoder endEncoding];
                [command_buffer presentDrawable:drawable];
                [command_buffer commit];
            }
        }

        app.shutdown();

        ImGui_ImplMetal_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    return 0;
}
