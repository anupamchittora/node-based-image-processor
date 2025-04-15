#pragma once
#include <unordered_map>
#include <vector>
#include <opencv2/opencv.hpp>
#include "BaseNode.h"

class NodeGraph {
public:
    std::unordered_map<int, BaseNode*> nodes;   // ?? Critical for fast lookup
    std::vector<std::pair<int, int>> connections;

    void AddNode(BaseNode* node);
    void Connect(int fromID, int toID);
    void ProcessAll();
};
