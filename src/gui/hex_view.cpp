#include "hex_view.h"

#include <imgui.h>

#include <cctype>
#include <cstddef>

namespace gui {

void draw_hex_view(const Packet* packet) {
    if (!packet) {
        ImGui::TextDisabled("No packet selected");
        return;
    }

    ImGui::Text("Seq: %llu  Bus: %u  Addr: %u  EP: %u  Len: %zu",
        static_cast<unsigned long long>(packet->seq),
        packet->bus,
        packet->addr,
        packet->ep,
        packet->raw.size());
    ImGui::Separator();

    ImGui::BeginChild("hex", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    for (std::size_t offset = 0; offset < packet->raw.size(); offset += 16) {
        ImGui::Text("%08zx  ", offset);
        ImGui::SameLine();

        for (std::size_t i = 0; i < 16; ++i) {
            const auto index = offset + i;
            if (index < packet->raw.size()) {
                ImGui::Text("%02X", packet->raw[index]);
            } else {
                ImGui::TextUnformatted("  ");
            }

            if (i != 15) {
                ImGui::SameLine();
            }
        }

        ImGui::SameLine();
        ImGui::TextUnformatted(" |");
        ImGui::SameLine();

        for (std::size_t i = 0; i < 16 && offset + i < packet->raw.size(); ++i) {
            const auto c = packet->raw[offset + i];
            const char text[2] = {
                static_cast<char>(std::isprint(c) ? c : '.'),
                '\0'
            };
            ImGui::TextUnformatted(text);
            if (i != 15) {
                ImGui::SameLine();
            }
        }
    }

    ImGui::EndChild();
}

}
