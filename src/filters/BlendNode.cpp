#include "filters/BlendNode.h"
#include <opencv2/opencv.hpp>
#include <iostream>

BlendNode::BlendNode(int id)
    : BaseNode(id, "Blend") {
}

void BlendNode::SetInputA(const cv::Mat& img) {
    inputA = img;
    Process();
}

void BlendNode::SetInputB(const cv::Mat& img) {
    inputB = img;
    Process();
}

void BlendNode::Process() {
    if (inputA.empty() || inputB.empty()) {
        std::cerr << "[BlendNode] One or both inputs are missing.\n";
        return;
    }

    cv::Mat a, b;
    cv::resize(inputA, a, inputB.size());
    inputB.copyTo(b);

    // Convert to 3-channel float
    if (a.channels() == 1) cv::cvtColor(a, a, cv::COLOR_GRAY2BGR);
    if (b.channels() == 1) cv::cvtColor(b, b, cv::COLOR_GRAY2BGR);
    a.convertTo(a, CV_32F);
    b.convertTo(b, CV_32F);

    cv::Mat result;

    switch (mode) {
    case BlendMode::Normal:
        result = a * (1.0f - opacity) + b * opacity;
        break;
    case BlendMode::Multiply:
        cv::multiply(a, b, result, 1.0 / 255.0);
        result = a * (1.0f - opacity) + result * opacity;
        break;
    case BlendMode::Screen:
        result = 255 - ((255 - a).mul(255 - b)) / 255;
        result = a * (1.0f - opacity) + result * opacity;
        break;
    case BlendMode::Overlay:
        result = a.clone();
        for (int y = 0; y < a.rows; ++y) {
            for (int x = 0; x < a.cols; ++x) {
                for (int c = 0; c < a.channels(); ++c) {
                    float valA = a.at<cv::Vec3f>(y, x)[c] / 255.0f;
                    float valB = b.at<cv::Vec3f>(y, x)[c] / 255.0f;
                    float val = valA < 0.5f ?
                        2.0f * valA * valB :
                        1.0f - 2.0f * (1.0f - valA) * (1.0f - valB);
                    result.at<cv::Vec3f>(y, x)[c] = 255.0f * ((1.0f - opacity) * valA + opacity * val);
                }
            }
        }
        break;
    case BlendMode::Difference:
        cv::absdiff(a, b, result);
        result = a * (1.0f - opacity) + result * opacity;
        break;
    }

    result.convertTo(outputImage, CV_8U);
    std::cout << "[BlendNode] Processed blend mode with opacity " << opacity << "\n";
}

