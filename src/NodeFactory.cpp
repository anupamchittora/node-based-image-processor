#include "NodeFactory.h"

BaseNode* CreateNodeByName(const std::string& name, int id) {
    if (name == "Image Input") return new ImageInputNode(id);
    if (name == "Output Node") return new OutputNode(id);
    if (name == "Grayscale") return new GrayscaleNode(id);
    if (name == "Brightness/Contrast") return new BrightnessContrastNode(id);
    if (name == "Channel Splitter") return new ColorChannelSplitterNode(id);
    if (name == "Gaussian Blur") return new BlurNode(id);
    if (name == "Threshold") return new ThresholdNode(id);
    if (name == "Edge Detection") return new EdgeDetectionNode(id);
    if (name == "Blend") return new BlendNode(id);
    if (name == "Noise Generator") return new NoiseNode(id);
    return nullptr;
}
