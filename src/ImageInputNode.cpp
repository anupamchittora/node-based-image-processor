#include "ImageInputNode.h"
#include <iostream>

ImageInputNode::ImageInputNode(int id)
    : BaseNode(id, "Image Input") {
}

void ImageInputNode::LoadImage(const std::string& path) {
    image = cv::imread(path);
    if (image.empty()) {
        std::cerr << "Failed to load image: " << path << std::endl;
    }
}

void ImageInputNode::Process() {
    // Nothing to process — image is already loaded
}
