#include "packet_list.h"

#include "../app.h"
#include "../usb/usb_parser.h"

#include <imgui.h>

#include <string>

namespace gui {

void draw_packet_list(App& app) {
    auto& packets = app.packets();

    if (ImGui::BeginTable("packets", 7,
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_Borders |
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_SizingStretchProp)) {

        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("Seq", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed, 110.0f);
        ImGui::TableSetupColumn("Device");
        ImGui::TableSetupColumn("EP", ImGuiTableColumnFlags_WidthFixed, 48.0f);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 60.0f);
        ImGui::TableSetupColumn("Len", ImGuiTableColumnFlags_WidthFixed, 60.0f);
        ImGui::TableSetupColumn("Summary");
        ImGui::TableHeadersRow();

        ImGuiListClipper clipper;
        clipper.Begin(static_cast<int>(packets.size()));

        while (clipper.Step()) {
            for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row) {
                const auto& p = packets[static_cast<std::size_t>(row)];

                ImGui::TableNextRow();
                ImGui::PushID(static_cast<int>(p.seq));

                ImGui::TableSetColumnIndex(0);
                if (ImGui::Selectable(std::to_string(p.seq).c_str(),
                    app.selected_index == row,
                    ImGuiSelectableFlags_SpanAllColumns)) {
                    app.selected_index = row;
                }

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.3f", p.timestamp_sec);

                ImGui::TableSetColumnIndex(2);
                ImGui::TextUnformatted(p.device_name.c_str());

                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%u", p.ep);

                ImGui::TableSetColumnIndex(4);
                ImGui::TextUnformatted(usb::transfer_type_name(p.xfer_type));

                ImGui::TableSetColumnIndex(5);
                ImGui::Text("%zu", p.raw.size());

                ImGui::TableSetColumnIndex(6);
                ImGui::TextUnformatted(p.summary.c_str());

                ImGui::PopID();
            }
        }

        ImGui::EndTable();
    }
}

}
