#include "NodeGraph.h"
#include "filters/GrayscaleNode.h"
#include "filters/BrightnessContrastNode.h"
#include "OutputNode.h"
#include "ImageInputNode.h"
#include "filters/ColorChannelSplitterNode.h"
#include "filters/BlurNode.h"
#include "filters/ThresholdNode.h"
#include "filters/EdgeDetectionNode.h"
#include "filters/BlendNode.h"
#include "filters/NoiseNode.h"
#include "filters/ConvolutionNode.h"

// ✅ This is the correct file for these
void NodeGraph::AddNode(BaseNode* node) {
    nodes[node->id] = node;
}

void NodeGraph::Connect(int fromID, int toID) {
    connections.push_back({ fromID, toID });

    BaseNode* from = nodes[fromID];
    BaseNode* to = nodes[toID];

    if (!from || !to) return;

    // 🔄 Handle BlendNode's multiple inputs
    if (BlendNode* blend = dynamic_cast<BlendNode*>(to)) {
        if (blend->inputA.empty())
            blend->inputA = from->GetOutput();
        else
            blend->inputB = from->GetOutput();
    }
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
        else if (NoiseNode* noise = dynamic_cast<NoiseNode*>(fromNode))
            output = &noise->outputImage;
        else if (ConvolutionNode* conv = dynamic_cast<ConvolutionNode*>(fromNode))
            output = &conv->outputImage;

        if (auto* conv = dynamic_cast<ConvolutionNode*>(toNode)) conv->SetInput(*output);
        else if (auto* blur = dynamic_cast<BlurNode*>(toNode)) blur->SetInput(*output);
        else if (auto* edge = dynamic_cast<EdgeDetectionNode*>(toNode)) edge->SetInput(*output);
        else if (auto* bc = dynamic_cast<BrightnessContrastNode*>(toNode)) bc->SetInput(*output);
        else if (auto* gray = dynamic_cast<GrayscaleNode*>(toNode)) gray->SetInput(*output);
        else if (auto* th = dynamic_cast<ThresholdNode*>(toNode)) th->SetInput(*output);
        else if (auto* out = dynamic_cast<OutputNode*>(toNode)) out->SetInput(*output);
        else if (auto* split = dynamic_cast<ColorChannelSplitterNode*>(toNode)) split->SetInput(*output);
        // skip NoiseNode and ImageInputNode as they don’t accept input


    }
}
