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

struct PendingConnection {
    bool isDragging = false;
    int fromNodeID = -1;
    ImVec2 startPos;
};
extern PendingConnection pendingConnection;
class NodeUIManager {
public:
    std::unordered_map<int, NodeUIState> nodeStates;
    std::vector<ConnectionVisual> connections;
    PendingConnection pendingConnection;
    void SpawnNode(const std::string& type);

    void RenderNode(BaseNode& node);
    void RenderConnections();
};
