#include "filters/GrayscaleNode.h"
#include <opencv2/imgproc.hpp>
#include <iostream>

GrayscaleNode::GrayscaleNode(int id)
    : BaseNode(id, "Grayscale") {
}

void GrayscaleNode::SetInput(const cv::Mat& image) {
    inputImage = image;
    Process();
}

void GrayscaleNode::Process() {
    if (inputImage.empty()) {
        std::cerr << "[GrayscaleNode] No input image to process.\n";
        return;
    }

    if (inputImage.channels() == 3) {
        cv::cvtColor(inputImage, outputImage, cv::COLOR_BGR2GRAY);
    }
    else if (inputImage.channels() == 4) {
        cv::cvtColor(inputImage, outputImage, cv::COLOR_BGRA2GRAY);
    }
    else {
        outputImage = inputImage.clone(); // Already grayscale
    }

    std::cout << "[GrayscaleNode] Converted to grayscale.\n";
}