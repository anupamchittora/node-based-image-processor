#pragma once
#include <unordered_map>
#include <string>
#include <imgui.h>
#include <vector>
#include "BaseNode.h"

struct NodeUIState {
    ImVec2 position;
    bool beingDragged = false;
};

struct ConnectionVisual {
    int fromID;
    int toID;
};

class NodeUIManager {
public:
    std::unordered_map<int, NodeUIState> nodeStates;
    std::vector<ConnectionVisual> connections;

    void RenderNode(BaseNode& node);

    void RenderConnections();
};