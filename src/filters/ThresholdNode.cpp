#include "filters/ThresholdNode.h"
#include <opencv2/imgproc.hpp>
#include <iostream>

ThresholdNode::ThresholdNode(int id)
    : BaseNode(id, "Threshold") {
}

void ThresholdNode::SetInput(const cv::Mat& image) {
    inputImage = image;
    Process();
}

void ThresholdNode::Process() {
    if (inputImage.empty()) {
        std::cerr << "[ThresholdNode] No input image.\n";
        return;
    }

    cv::Mat gray;
    if (inputImage.channels() == 3 || inputImage.channels() == 4) {
        cv::cvtColor(inputImage, gray, cv::COLOR_BGR2GRAY);
    }
    else {
        gray = inputImage;
    }

    cv::threshold(gray, outputImage, thresholdValue, 255, cv::THRESH_BINARY);
    std::cout << "[ThresholdNode] Applied binary threshold at value = " << thresholdValue << "\n";
}
