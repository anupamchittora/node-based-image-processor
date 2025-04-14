#include "filters/ConvolutionNode.h"
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <unordered_map>

ConvolutionNode::ConvolutionNode(int id)
    : BaseNode(id, "Convolution") {
    kernel = {
        0, -1, 0,
       -1,  5, -1,
        0, -1, 0
    };
}

void ConvolutionNode::SetInput(const cv::Mat& image) {
    inputImage = image;
    Process();
}

void ConvolutionNode::SetPreset(const std::string& name) {
    if (name == "Sharpen") {
        kernel = { 0, -1, 0, -1, 5, -1, 0, -1, 0 };
        kernelSize = 3;
    }
    else if (name == "Edge") {
        kernel = { -1, -1, -1, -1, 8, -1, -1, -1, -1 };
        kernelSize = 3;
    }
    else if (name == "Emboss") {
        kernel = { -2, -1, 0, -1, 1, 1, 0, 1, 2 };
        kernelSize = 3;
    }
}

void ConvolutionNode::Process() {
    if (inputImage.empty()) {
        std::cerr << "[ConvolutionNode] No input image.\n";
        return;
    }

    cv::Mat kernelMat(kernel);
    kernelMat = kernelMat.reshape(1, kernelSize); // Make 2D

    cv::filter2D(inputImage, outputImage, -1, kernelMat);
    std::cout << "[ConvolutionNode] Applied custom kernel (" << kernelSize << "x" << kernelSize << ")\n";
}
