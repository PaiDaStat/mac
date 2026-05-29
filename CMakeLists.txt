cmake_minimum_required(VERSION 3.20)
project(MacUSBMon LANGUAGES C CXX OBJCXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Build universal macOS binary")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(glfw3 REQUIRED)
find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
find_library(IOKIT_FRAMEWORK IOKit REQUIRED)
find_library(COREVIDEO_FRAMEWORK CoreVideo REQUIRED)
find_library(METAL_FRAMEWORK Metal REQUIRED)
find_library(QUARTZCORE_FRAMEWORK QuartzCore REQUIRED)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui)
set(IMPLOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/implot)

if(NOT EXISTS "${IMGUI_DIR}/imgui.cpp")
    message(FATAL_ERROR "Dear ImGui not found. Clone it into third_party/imgui.")
endif()

if(NOT EXISTS "${IMPLOT_DIR}/implot.cpp")
    message(FATAL_ERROR "ImPlot not found. Clone it into third_party/implot.")
endif()

add_executable(MacUSBMon MACOSX_BUNDLE
    src/main.mm
    src/app.cpp
    src/usb/dext_client.cpp
    src/usb/ring_buffer.cpp
    src/usb/usb_parser.cpp
    src/usb/hid_decoder.cpp
    src/gui/main_window.cpp
    src/gui/packet_list.cpp
    src/gui/hex_view.cpp
    src/gui/device_tree.cpp
    src/utils/pcap_exporter.cpp
    third_party/imgui/imgui.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_tables.cpp
    third_party/imgui/imgui_widgets.cpp
    third_party/imgui/imgui_demo.cpp
    third_party/imgui/backends/imgui_impl_glfw.cpp
    third_party/imgui/backends/imgui_impl_metal.mm
    third_party/implot/implot.cpp
    third_party/implot/implot_items.cpp
)

target_include_directories(MacUSBMon PRIVATE
    src
    third_party/imgui
    third_party/imgui/backends
    third_party/implot
)

target_link_libraries(MacUSBMon PRIVATE
    glfw
    ${COCOA_FRAMEWORK}
    ${IOKIT_FRAMEWORK}
    ${COREVIDEO_FRAMEWORK}
    ${METAL_FRAMEWORK}
    ${QUARTZCORE_FRAMEWORK}
)

target_compile_options(MacUSBMon PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -Wpedantic>
    $<$<COMPILE_LANGUAGE:OBJCXX>:-Wall -Wextra>
)

set_target_properties(MacUSBMon PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER com.local.macusbmon
)
