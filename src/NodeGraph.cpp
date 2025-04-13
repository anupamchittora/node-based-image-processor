#include "NodeGraph.h"

std::shared_ptr<BaseNode> NodeGraph::AddNode(std::shared_ptr<BaseNode> node) {
    node->id = nextID++;
    nodes.push_back(node);
    return node;
}

void NodeGraph::Connect(int fromID, int toID) {
    connections.emplace_back(fromID, toID);
}

void NodeGraph::ProcessAll() {
    for (auto& node : nodes) {
        node->Process();
    }
}
