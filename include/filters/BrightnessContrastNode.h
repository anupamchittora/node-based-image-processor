#pragma once
#include "BaseNode.h"
#include <opencv2/opencv.hpp>

class BrightnessContrastNode : public BaseNode {
public:
    cv::Mat inputImage;
    cv::Mat outputImage;

    float brightness = 0.0f;  // Range: -100 to +100
    float contrast = 1.0f;    // Range: 0 to 3

    BrightnessContrastNode(int id);
    void SetInput(const cv::Mat& image);
    void Process() override;
};
