#include "NodeGraph.h"

void NodeGraph::AddNode(BaseNode* node) {
    nodes[node->id] = node;
}

void NodeGraph::Connect(int fromID, int toID) {
    connections.push_back({ fromID, toID });
}

void NodeGraph::ProcessAll() {
    for (auto& [fromID, toID] : connections) {
        BaseNode* fromNode = nodes[fromID];
        BaseNode* toNode = nodes[toID];

        ImageInputNode* input = dynamic_cast<ImageInputNode*>(fromNode);
        OutputNode* output = dynamic_cast<OutputNode*>(toNode);

        if (input && output) {
            output->SetInput(input->outputImage);
        }
    }
}
