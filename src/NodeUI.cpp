#include "NodeUI.h"
#include <iostream>
#include "OutputNode.h"
void NodeUIManager::RenderNode(BaseNode& node) {
    int id = node.id;
    std::string name = node.name;
    NodeUIState& state = nodeStates[id];

    if (state.position.x == 0 && state.position.y == 0)
        state.position = ImVec2(100 + id * 180, 250);

    ImVec2 nodeSize = ImVec2(200, 140);
    ImVec2 start = state.position;
    ImVec2 end = ImVec2(start.x + nodeSize.x, start.y + nodeSize.y);

    ImGui::SetCursorScreenPos(start);
    ImGui::PushID(id);
    ImGui::BeginGroup();

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(start, end, IM_COL32(40, 40, 50, 255), 6.0f);
    drawList->AddRect(start, end, IM_COL32(255, 255, 255, 255), 6.0f);
    drawList->AddText(ImVec2(start.x + 10, start.y + 10), IM_COL32(255, 255, 255, 255), name.c_str());

    // Output pin (right side)
    ImVec2 outputPin = ImVec2(end.x - 10, start.y + nodeSize.y * 0.5f);
    drawList->AddCircleFilled(outputPin, 5.0f, IM_COL32(200, 200, 0, 255));
    ImGui::SetCursorScreenPos(ImVec2(outputPin.x - 5, outputPin.y - 5));

    ImGui::InvisibleButton("output_pin", ImVec2(10, 10));
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        pendingConnection.isDragging = true;
        pendingConnection.fromNodeID = id;
        pendingConnection.startPos = outputPin;
    }

    // Input pin (left side)
    ImVec2 inputPin = ImVec2(start.x, start.y + nodeSize.y * 0.5f);
    drawList->AddCircleFilled(inputPin, 5.0f, IM_COL32(0, 200, 200, 255));
    ImGui::SetCursorScreenPos(ImVec2(inputPin.x - 5, inputPin.y - 5));

    

    ImGui::InvisibleButton("input_pin", ImVec2(10, 10));
    if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && pendingConnection.isDragging) {
        connections.push_back({ pendingConnection.fromNodeID, id });
        pendingConnection = PendingConnection();
    }

    // Output preview (optional)
    if (OutputNode* outNode = dynamic_cast<OutputNode*>(&node)) {
        if (outNode->textureID) {
            ImTextureID texID = (ImTextureID)(intptr_t)outNode->textureID;
            ImGui::SetCursorScreenPos(ImVec2(start.x + 10, start.y + 30));
            ImGui::Image(texID, ImVec2(180, 100));
        }
    }

    ImGui::SetCursorScreenPos(start);
    ImGui::InvisibleButton("node_drag", nodeSize);

    bool nodeHovered = ImGui::IsItemHovered();
    bool nodeHeld = ImGui::IsItemActive();

    if (nodeHeld && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        state.position.x += ImGui::GetIO().MouseDelta.x;
        state.position.y += ImGui::GetIO().MouseDelta.y;
    }


    ImGui::EndGroup();
    ImGui::PopID();
}

void NodeUIManager::RenderConnections() {
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    for (const auto& conn : connections) {
        ImVec2 fromPos = nodeStates[conn.fromID].position;
        ImVec2 toPos = nodeStates[conn.toID].position;

        ImVec2 p1 = ImVec2(fromPos.x + 200, fromPos.y + 70);
        ImVec2 p2 = ImVec2(toPos.x, toPos.y + 70);

        ImVec2 cp1 = ImVec2(p1.x + 50, p1.y);
        ImVec2 cp2 = ImVec2(p2.x - 50, p2.y);

        drawList->AddBezierCubic(p1, cp1, cp2, p2, IM_COL32(200, 200, 100, 255), 3.0f);
    }

    // If dragging wire
    if (pendingConnection.isDragging) {
        ImVec2 mousePos = ImGui::GetIO().MousePos;
        ImVec2 cp1 = ImVec2(pendingConnection.startPos.x + 50, pendingConnection.startPos.y);
        ImVec2 cp2 = ImVec2(mousePos.x - 50, mousePos.y);

        drawList->AddBezierCubic(pendingConnection.startPos, cp1, cp2, mousePos,
            IM_COL32(255, 255, 0, 255), 2.0f);

        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            pendingConnection = PendingConnection();
        }
    }
}