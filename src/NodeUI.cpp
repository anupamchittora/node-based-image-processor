#include "NodeUI.h"

void NodeUIManager::RenderNode(int id, const std::string& name) {
    NodeUIState& state = nodeStates[id];

    ImGui::SetCursorScreenPos(state.position);
    ImGui::PushID(id);
    ImGui::BeginGroup();

    ImGui::BeginChild("node_frame", ImVec2(150, 60), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::Text("%s", name.c_str());
    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    ImGui::EndChild();

    ImGui::EndGroup();

    // Drag support
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        state.position.x += ImGui::GetIO().MouseDelta.x;
        state.position.y += ImGui::GetIO().MouseDelta.y;
    }

    ImGui::PopID();
}
