#include "NodeGraph.h"
#include "filters/GrayscaleNode.h"

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

        if (!fromNode || !toNode) continue;

        cv::Mat* output = nullptr;

        if (ImageInputNode* input = dynamic_cast<ImageInputNode*>(fromNode))
            output = &input->outputImage;
        else if (GrayscaleNode* gray = dynamic_cast<GrayscaleNode*>(fromNode))
            output = &gray->outputImage;

        if (output && !output->empty()) {
            if (GrayscaleNode* grayTarget = dynamic_cast<GrayscaleNode*>(toNode))
                grayTarget->SetInput(*output);
            else if (OutputNode* out = dynamic_cast<OutputNode*>(toNode))
                out->SetInput(*output);
        }
    }
}

