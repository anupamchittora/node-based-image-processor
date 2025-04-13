#include "NodeUI.h"
#include <iostream>

void NodeUIManager::RenderNode(int id, const std::string& name) {
    NodeUIState& state = nodeStates[id];

    if (state.position.x == 0 && state.position.y == 0)
        state.position = ImVec2(100 + id * 180, 250);

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 nodeSize = ImVec2(150, 60);

    ImVec2 start = state.position;
    ImVec2 end = ImVec2(start.x + nodeSize.x, start.y + nodeSize.y);

    ImGui::SetCursorScreenPos(start);
    ImGui::PushID(id);

    // Draw node background
    drawList->AddRectFilled(start, end, IM_COL32(50, 50, 60, 255), 5.0f);
    drawList->AddRect(start, end, IM_COL32(255, 255, 255, 255), 5.0f);
    drawList->AddText(ImVec2(start.x + 10, start.y + 10), IM_COL32(255, 255, 255, 255), name.c_str());

    // Drag behavior
    ImGui::SetCursorScreenPos(start);
    ImGui::InvisibleButton("node", nodeSize);
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        state.position.x += ImGui::GetIO().MouseDelta.x;
        state.position.y += ImGui::GetIO().MouseDelta.y;
    }

    ImGui::PopID();
}

void NodeUIManager::RenderConnections() {
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    for (const auto& conn : connections) {
        ImVec2 fromPos = nodeStates[conn.fromID].position;
        ImVec2 toPos = nodeStates[conn.toID].position;

        ImVec2 p1 = ImVec2(fromPos.x + 150, fromPos.y + 30); // Right center of source
        ImVec2 p2 = ImVec2(toPos.x, toPos.y + 30);           // Left center of target

        ImVec2 cp1 = ImVec2(p1.x + 50, p1.y);
        ImVec2 cp2 = ImVec2(p2.x - 50, p2.y);

        drawList->AddBezierCubic(p1, cp1, cp2, p2, IM_COL32(200, 200, 100, 255), 3.0f);



    }
}
