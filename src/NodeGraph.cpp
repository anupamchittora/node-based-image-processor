#include "NodeGraph.h"
#include "filters/GrayscaleNode.h"
#include "filters/BrightnessContrastNode.h"
#include "OutputNode.h"
#include "ImageInputNode.h"
#include "filters/ColorChannelSplitterNode.h"
#include "filters/BlurNode.h"
#include "filters/ThresholdNode.h"
#include "filters/EdgeDetectionNode.h"

// ✅ This is the correct file for these
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
        else if (ColorChannelSplitterNode* splitter = dynamic_cast<ColorChannelSplitterNode*>(fromNode))
            output = &splitter->rChannel;  // Default to red channel for now
        else if (BlurNode* blur = dynamic_cast<BlurNode*>(fromNode))
            output = &blur->outputImage;
        else if (ThresholdNode* th = dynamic_cast<ThresholdNode*>(fromNode))
            output = &th->outputImage;
        else if (EdgeDetectionNode* edge = dynamic_cast<EdgeDetectionNode*>(fromNode))
            output = &edge->outputImage;
        if (ThresholdNode* thTarget = dynamic_cast<ThresholdNode*>(toNode))
            thTarget->SetInput(*output);
        if (BlurNode* blurTarget = dynamic_cast<BlurNode*>(toNode))
            blurTarget->SetInput(*output);
        if (EdgeDetectionNode* edgeTarget = dynamic_cast<EdgeDetectionNode*>(toNode))
            edgeTarget->SetInput(*output);
        if (ColorChannelSplitterNode* target = dynamic_cast<ColorChannelSplitterNode*>(toNode))
            target->SetInput(*output);

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
