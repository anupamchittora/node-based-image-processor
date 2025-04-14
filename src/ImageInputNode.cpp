// ImageInputNode.cpp
#include "ImageInputNode.h"
#include <iostream>

ImageInputNode::ImageInputNode(int id)
    : BaseNode(id, "Image Input") {
}

void ImageInputNode::SetImagePath(const std::string& path) {
    imagePath = path;
    Process();
}

void ImageInputNode::Process() {
    outputImage = cv::imread(imagePath);
    if (outputImage.empty()) {
        std::cerr << "[ImageInputNode] Failed to load image: " << imagePath << std::endl;
    }
    else {
        std::cout << "[ImageInputNode] Loaded image: " << imagePath << std::endl;
    }
}