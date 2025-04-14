#include "filters/EdgeDetectionNode.h"
#include <opencv2/imgproc.hpp>
#include <iostream>

EdgeDetectionNode::EdgeDetectionNode(int id)
    : BaseNode(id, "Edge Detection") {
}

void EdgeDetectionNode::SetInput(const cv::Mat& image) {
    inputImage = image;
    Process();
}

void EdgeDetectionNode::Process() {
    if (inputImage.empty()) {
        std::cerr << "[EdgeDetectionNode] No input image.\n";
        return;
    }

    cv::Mat gray;
    if (inputImage.channels() == 3 || inputImage.channels() == 4) {
        cv::cvtColor(inputImage, gray, cv::COLOR_BGR2GRAY);
    }
    else {
        gray = inputImage;
    }

    if (mode == EdgeMode::Sobel) {
        cv::Mat gradX, gradY;
        cv::Sobel(gray, gradX, CV_16S, 1, 0, kernelSize);
        cv::Sobel(gray, gradY, CV_16S, 0, 1, kernelSize);
        cv::Mat absX, absY;
        cv::convertScaleAbs(gradX, absX);
        cv::convertScaleAbs(gradY, absY);
        cv::addWeighted(absX, 0.5, absY, 0.5, 0, outputImage);
    }
    else if (mode == EdgeMode::Canny) {
        cv::Canny(gray, outputImage, cannyThreshold1, cannyThreshold2);
    }

    std::cout << "[EdgeDetectionNode] Applied " << (mode == EdgeMode::Sobel ? "Sobel" : "Canny") << " edge detection\n";
}
