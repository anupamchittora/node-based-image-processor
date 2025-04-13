#pragma once
#include "BaseNode.h"
#include "Connection.h"
#include <vector>
#include <memory>

class NodeGraph {
public:
    std::vector<std::shared_ptr<BaseNode>> nodes;
    std::vector<Connection> connections;
    int nextID = 1;

    std::shared_ptr<BaseNode> AddNode(std::shared_ptr<BaseNode> node);
    void Connect(int fromID, int toID);
    void ProcessAll();
};
