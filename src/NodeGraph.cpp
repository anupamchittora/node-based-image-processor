#include "NodeGraph.h"
#include "filters/GrayscaleNode.h"
#include "filters/BrightnessContrastNode.h"
#include "OutputNode.h"
#include "ImageInputNode.h"

// ? This is the correct file for these
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
        else if (BrightnessContrastNode* bc = dynamic_cast<BrightnessContrastNode*>(fromNode))
            output = &bc->outputImage;

        if (output && !output->empty()) {
            if (BrightnessContrastNode* bcTarget = dynamic_cast<BrightnessContrastNode*>(toNode))
                bcTarget->SetInput(*output);
            else if (GrayscaleNode* grayTarget = dynamic_cast<GrayscaleNode*>(toNode))
                grayTarget->SetInput(*output);
            else if (OutputNode* out = dynamic_cast<OutputNode*>(toNode))
                out->SetInput(*output);
        }
    }
}
