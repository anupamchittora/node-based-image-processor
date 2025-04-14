#include "NodeUI.h"
#include <iostream>
#include "OutputNode.h"
#include <imgui_internal.h>
#include "filters/BrightnessContrastNode.h"
#include "filters/BlurNode.h"

#include "NodeGraph.h"
PendingConnection pendingConnection;
extern NodeGraph graph;

void NodeUIManager::RenderNode(BaseNode& node) {
    int id = node.id;
    std::string name = node.name;
    NodeUIState& state = nodeStates[id];

    if (state.position.x == 0 && state.position.y == 0)
        state.position = ImVec2(100 + id * 180, 250);

    ImVec2 nodeSize = ImVec2(200, 180);
    ImVec2 start = state.position;
    ImVec2 end = ImVec2(start.x + nodeSize.x, start.y + nodeSize.y);

    ImGui::SetCursorScreenPos(start);
    ImGui::PushID(id);
    ImGui::BeginGroup();

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(start, end, IM_COL32(40, 40, 50, 255), 6.0f);
    drawList->AddRect(start, end, IM_COL32(255, 255, 255, 255), 6.0f);
    drawList->AddText(ImVec2(start.x + 10, start.y + 10), IM_COL32(255, 255, 255, 255), name.c_str());

    // Output preview (optional)
    if (OutputNode* outNode = dynamic_cast<OutputNode*>(&node)) {
        if (outNode->textureID) {
            ImTextureID texID = (ImTextureID)(intptr_t)outNode->textureID;
            ImGui::SetCursorScreenPos(ImVec2(start.x + 10, start.y + 30));
            ImGui::Image(texID, ImVec2(180, 100));
        }
    }
    if (BrightnessContrastNode* bc = dynamic_cast<BrightnessContrastNode*>(&node)) {
        ImGui::SetCursorScreenPos(ImVec2(start.x + 10, start.y + 30));
        bool updated = false;

        ImGui::Text("Brightness");
        updated |= ImGui::SliderFloat(("##brightness_" + std::to_string(id)).c_str(), &bc->brightness, -100.0f, 100.0f);
        if (ImGui::Button(("Reset B##" + std::to_string(id)).c_str())) {
            bc->brightness = 0.0f;
            updated = true;
        }

        ImGui::Text("Contrast");
        updated |= ImGui::SliderFloat(("##contrast_" + std::to_string(id)).c_str(), &bc->contrast, 0.0f, 3.0f);
        if (ImGui::Button(("Reset C##" + std::to_string(id)).c_str())) {
            bc->contrast = 1.0f;
            updated = true;
        }

        if (updated) {
            bc->Process();
            graph.ProcessAll();
        }
    }
    if (BlurNode* blur = dynamic_cast<BlurNode*>(&node)) {
        ImGui::SetCursorScreenPos(ImVec2(start.x + 10, start.y + 30));

        ImGui::Text("Radius");
        if (ImGui::SliderInt(("##blur_" + std::to_string(id)).c_str(), &blur->radius, 1, 20)) {
            blur->Process();
            graph.ProcessAll();
        }
    }


    ImGui::SetCursorScreenPos(start);
    ImGui::InvisibleButton("node_drag", nodeSize);

    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
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

        ImVec2 p1 = ImVec2(fromPos.x + 200, fromPos.y + 70); // Right middle of source
        ImVec2 p2 = ImVec2(toPos.x, toPos.y + 70);           // Left middle of target

        ImVec2 cp1 = ImVec2(p1.x + 50, p1.y);
        ImVec2 cp2 = ImVec2(p2.x - 50, p2.y);

        drawList->AddBezierCubic(p1, cp1, cp2, p2, IM_COL32(200, 200, 100, 255), 3.0f);
    }

    // Render interactive pins and connection logic
    for (const auto& [id, state] : nodeStates) {
        ImVec2 outPin = ImVec2(state.position.x + 200, state.position.y + 70);
        ImVec2 inPin = ImVec2(state.position.x, state.position.y + 70);

        // Output pin
        ImGui::SetCursorScreenPos(ImVec2(outPin.x - 5, outPin.y - 5));
        ImGui::InvisibleButton(("out_" + std::to_string(id)).c_str(), ImVec2(10, 10));
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            pendingConnection.isDragging = true;
            pendingConnection.fromNodeID = id;
            pendingConnection.startPos = outPin;
        }

        // Input pin
        ImGui::SetCursorScreenPos(ImVec2(inPin.x - 5, inPin.y - 5));
        ImGui::InvisibleButton(("in_" + std::to_string(id)).c_str(), ImVec2(10, 10));
        if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && pendingConnection.isDragging) {
            connections.push_back({ pendingConnection.fromNodeID, id });
            graph.Connect(pendingConnection.fromNodeID, id);
            graph.ProcessAll();
            pendingConnection = PendingConnection();
        }

        // Draw pins
        drawList->AddCircleFilled(outPin, 5.0f, IM_COL32(255, 255, 0, 255));
        drawList->AddCircleFilled(inPin, 5.0f, IM_COL32(0, 255, 255, 255));
    }

    // Render live dragging connection
    if (pendingConnection.isDragging) {
        ImVec2 mousePos = ImGui::GetIO().MousePos;
        ImVec2 p1 = pendingConnection.startPos;
        ImVec2 p2 = mousePos;
        ImVec2 cp1 = ImVec2(p1.x + 50, p1.y);
        ImVec2 cp2 = ImVec2(p2.x - 50, p2.y);
        drawList->AddBezierCubic(p1, cp1, cp2, p2, IM_COL32(255, 255, 0, 255), 2.0f);
    }
    if (pendingConnection.isDragging && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        pendingConnection = PendingConnection(); // Reset state
    }


}
