#include "filters/BlurNode.h"
#include <opencv2/opencv.hpp>
#include <iostream>

BlurNode::BlurNode(int id)
    : BaseNode(id, "Gaussian Blur") {
}

void BlurNode::SetInput(const cv::Mat& image) {
    inputImage = image;
    Process();
}

void BlurNode::Process() {
    if (inputImage.empty()) {
        std::cerr << "[BlurNode] No input image.\n";
        return;
    }

    int kernelSize = radius * 2 + 1;
    cv::GaussianBlur(inputImage, outputImage, cv::Size(kernelSize, kernelSize), 0);
    std::cout << "[BlurNode] Applied Gaussian blur (radius = " << radius << ")\n";
}
