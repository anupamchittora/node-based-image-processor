#include "filters/BrightnessContrastNode.h"
#include <opencv2/opencv.hpp>
#include <iostream>

BrightnessContrastNode::BrightnessContrastNode(int id)
    : BaseNode(id, "Brightness/Contrast") {
}

void BrightnessContrastNode::SetInput(const cv::Mat& image) {
    inputImage = image;
    Process();
}

void BrightnessContrastNode::Process() {
    if (inputImage.empty()) {
        std::cerr << "[BrightnessContrastNode] No input image.\n";
        return;
    }

    inputImage.convertTo(outputImage, -1, contrast, brightness);
    std::cout << "[BrightnessContrastNode] Applied brightness/contrast.\n";
}
