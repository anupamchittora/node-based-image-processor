#include "NodeUI.h"
#include <iostream>
#include "OutputNode.h"
#include <imgui_internal.h>
#include "filters/BrightnessContrastNode.h"
#include "filters/BlurNode.h"
#include "filters/ThresholdNode.h"
#include "filters/EdgeDetectionNode.h"
#include "filters/BlendNode.h"
#include "filters/NoiseNode.h" 
#include "NodeGraph.h"

PendingConnection pendingConnection;
extern NodeGraph graph;

void NodeUIManager::RenderNode(BaseNode& node) {
    int id = node.id;
    std::string name = node.name;
    NodeUIState& state = nodeStates[id];
    ImVec2 scrollOffset(ImGui::GetScrollX(), ImGui::GetScrollY());

    if (state.position.x == 0 && state.position.y == 0)
        state.position = ImVec2(100 + id * 180, 250);

    ImVec2 nodeSize = ImVec2(200, 180);
    ImVec2 start = ImVec2(state.position.x - scrollOffset.x, state.position.y - scrollOffset.y);
    ImVec2 end = ImVec2(start.x + nodeSize.x, start.y + nodeSize.y);

    ImGui::SetCursorScreenPos(start);
    ImGui::PushID(id);
    ImGui::BeginGroup();

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(start, end, IM_COL32(40, 40, 50, 255), 6.0f);
    drawList->AddRect(start, end, IM_COL32(255, 255, 255, 255), 6.0f);
    drawList->AddText(ImVec2(start.x + 10, start.y + 10), IM_COL32(255, 255, 255, 255), name.c_str());

    // ... keep existing UI logic as-is ...

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
    ImVec2 scrollOffset(ImGui::GetScrollX(), ImGui::GetScrollY());
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    for (const auto& conn : connections) {
        ImVec2 fromPos = nodeStates[conn.fromID].position;
        ImVec2 toPos = nodeStates[conn.toID].position;

        fromPos = ImVec2(fromPos.x - scrollOffset.x, fromPos.y - scrollOffset.y);
        toPos = ImVec2(toPos.x - scrollOffset.x, toPos.y - scrollOffset.y);

        ImVec2 p1 = ImVec2(fromPos.x + 200, fromPos.y + 70);
        ImVec2 p2 = ImVec2(toPos.x, toPos.y + 70);

        ImVec2 cp1 = ImVec2(p1.x + 50, p1.y);
        ImVec2 cp2 = ImVec2(p2.x - 50, p2.y);

        drawList->AddBezierCubic(p1, cp1, cp2, p2, IM_COL32(200, 200, 100, 255), 3.0f);
    }

    for (const auto& [id, state] : nodeStates) {
        ImVec2 outPin = ImVec2(state.position.x + 200 - scrollOffset.x, state.position.y + 70 - scrollOffset.y);
        ImGui::SetCursorScreenPos(ImVec2(outPin.x - 5, outPin.y - 5));
        ImGui::InvisibleButton(("out_" + std::to_string(id)).c_str(), ImVec2(10, 10));
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            pendingConnection.isDragging = true;
            pendingConnection.fromNodeID = id;
            pendingConnection.startPos = outPin;
        }

        BaseNode* toNode = graph.nodes[id];
        if (BlendNode* blend = dynamic_cast<BlendNode*>(toNode)) {
            ImVec2 inPinA = ImVec2(state.position.x - scrollOffset.x, state.position.y + 50 - scrollOffset.y);
            ImVec2 inPinB = ImVec2(state.position.x - scrollOffset.x, state.position.y + 90 - scrollOffset.y);

            ImGui::SetCursorScreenPos(ImVec2(inPinA.x - 5, inPinA.y - 5));
            ImGui::InvisibleButton(("inA_" + std::to_string(id)).c_str(), ImVec2(10, 10));
            if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && pendingConnection.isDragging) {
                blend->SetInputA(graph.nodes[pendingConnection.fromNodeID]->GetOutput());
                connections.push_back({ pendingConnection.fromNodeID, id });
                graph.ProcessAll();
                pendingConnection = PendingConnection();
            }

            ImGui::SetCursorScreenPos(ImVec2(inPinB.x - 5, inPinB.y - 5));
            ImGui::InvisibleButton(("inB_" + std::to_string(id)).c_str(), ImVec2(10, 10));
            if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && pendingConnection.isDragging) {
                blend->SetInputB(graph.nodes[pendingConnection.fromNodeID]->GetOutput());
                connections.push_back({ pendingConnection.fromNodeID, id });
                graph.ProcessAll();
                pendingConnection = PendingConnection();
            }

            drawList->AddCircleFilled(inPinA, 5.0f, IM_COL32(0, 255, 255, 255));
            drawList->AddCircleFilled(inPinB, 5.0f, IM_COL32(0, 255, 255, 255));
        }
        else {
            ImVec2 inPin = ImVec2(state.position.x - scrollOffset.x, state.position.y + 70 - scrollOffset.y);
            ImGui::SetCursorScreenPos(ImVec2(inPin.x - 5, inPin.y - 5));
            ImGui::InvisibleButton(("in_" + std::to_string(id)).c_str(), ImVec2(10, 10));
            if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && pendingConnection.isDragging) {
                connections.push_back({ pendingConnection.fromNodeID, id });
                graph.Connect(pendingConnection.fromNodeID, id);
                graph.ProcessAll();
                pendingConnection = PendingConnection();
            }
            drawList->AddCircleFilled(inPin, 5.0f, IM_COL32(0, 255, 255, 255));
        }

        drawList->AddCircleFilled(outPin, 5.0f, IM_COL32(255, 255, 0, 255));
    }

    if (pendingConnection.isDragging) {
        ImVec2 mousePos = ImGui::GetIO().MousePos;
        ImVec2 p1 = pendingConnection.startPos;
        ImVec2 p2 = mousePos;
        ImVec2 cp1 = ImVec2(p1.x + 50, p1.y);
        ImVec2 cp2 = ImVec2(p2.x - 50, p2.y);
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddBezierCubic(p1, cp1, cp2, p2, IM_COL32(255, 255, 0, 255), 2.0f);
    }
    if (pendingConnection.isDragging && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        pendingConnection = PendingConnection();
    }
}