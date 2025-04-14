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

    switch (mode) {
    case BlendMode::Normal:
        outputImage = a * (1.0f - opacity) + inputB * opacity;
        break;
    case BlendMode::Multiply:
        cv::multiply(a, inputB, outputImage, opacity / 255.0);
        break;
    case BlendMode::Screen:
        outputImage = 255 - ((255 - a).mul(255 - inputB)) / 255;
        outputImage.convertTo(outputImage, a.type(), opacity);
        break;
    case BlendMode::Overlay: {
        cv::Mat result;
        a.convertTo(a, CV_32F);
        inputB.convertTo(b, CV_32F);
        result = a.clone();
        for (int y = 0; y < a.rows; ++y) {
            for (int x = 0; x < a.cols; ++x) {
                for (int c = 0; c < a.channels(); ++c) {
                    float valA = a.at<cv::Vec3f>(y, x)[c] / 255.0f;
                    float valB = b.at<cv::Vec3f>(y, x)[c] / 255.0f;
                    float resultVal = valA < 0.5f ?
                        2.0f * valA * valB :
                        1.0f - 2.0f * (1.0f - valA) * (1.0f - valB);
                    result.at<cv::Vec3f>(y, x)[c] = 255.0f * resultVal * opacity;
                }
            }
        }
        result.convertTo(outputImage, a.type());
        break;
    }
    case BlendMode::Difference:
        cv::absdiff(a, inputB, outputImage);
        outputImage.convertTo(outputImage, a.type(), opacity);
        break;
    }

    std::cout << "[BlendNode] Processed blend mode with opacity " << opacity << "\n";
}
