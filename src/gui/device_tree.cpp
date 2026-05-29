#include "device_tree.h"

#include <imgui.h>

namespace gui {

void draw_device_tree() {
    if (ImGui::TreeNodeEx("Bus 001", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::TreeNodeEx("USB Keyboard", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::BulletText("Address 2");
            ImGui::BulletText("Interface 0: HID");
            ImGui::BulletText("Endpoint 1 IN: Interrupt");
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("USB Storage")) {
            ImGui::BulletText("Address 3");
            ImGui::BulletText("Interface 0: Mass Storage");
            ImGui::BulletText("Endpoint 2 OUT: Bulk");
            ImGui::BulletText("Endpoint 3 IN: Bulk");
            ImGui::TreePop();
        }

        ImGui::TreePop();
    }
}

}
