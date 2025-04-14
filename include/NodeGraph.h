#pragma once
#include <vector>
#include <unordered_map>
#include "BaseNode.h"
#include "ImageInputNode.h"
#include "OutputNode.h"

class NodeGraph {
public:
    std::unordered_map<int, BaseNode*> nodes;
    std::vector<std::pair<int, int>> connections; // from ? to

    void AddNode(BaseNode* node);
    void Connect(int fromID, int toID);
    void ProcessAll();
};