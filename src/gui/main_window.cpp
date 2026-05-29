#include "main_window.h"

#include "device_tree.h"
#include "hex_view.h"
#include "packet_list.h"
#include "../app.h"

#include <imgui.h>

namespace {

const char* default_export_path() {
    return "/tmp/macusbmon_capture.pcap";
}

}

namespace gui {

void draw_main_window(App& app) {
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Export PCAP")) {
                app.export_pcap(default_export_path());
            }
            ImGui::MenuItem("Export JSON", nullptr, false, false);
            ImGui::Separator();
            if (ImGui::MenuItem("Clear Packets")) {
                app.clear_packets();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Capture")) {
            if (ImGui::MenuItem("Start", nullptr, false, !app.is_capturing())) {
                app.start_capture();
            }
            if (ImGui::MenuItem("Stop", nullptr, false, app.is_capturing())) {
                app.stop_capture();
            }
            ImGui::EndMenu();
        }

        ImGui::Text("Packets: %zu", app.packets().size());
        ImGui::Text("Dropped: %llu", static_cast<unsigned long long>(app.dropped_packets()));

        ImGui::EndMainMenuBar();
    }

    ImGui::Begin("Devices");
    draw_device_tree();
    ImGui::End();

    ImGui::Begin("Packets");
    draw_packet_list(app);
    ImGui::End();

    ImGui::Begin("Hex View");

    Packet* packet = nullptr;
    if (app.selected_index >= 0 &&
        app.selected_index < static_cast<int>(app.packets().size())) {
        packet = &app.packets()[static_cast<std::size_t>(app.selected_index)];
    }

    draw_hex_view(packet);
    ImGui::End();
}

}
