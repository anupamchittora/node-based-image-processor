// NodeUI.h
#pragma once
#include <unordered_map>
#include <string>
#include <imgui.h>

struct NodeUIState {
    ImVec2 position;
    bool beingDragged = false;
};

class NodeUIManager {
public:
    std::unordered_map<int, NodeUIState> nodeStates;

    void RenderNode(int id, const std::string& name);
};
