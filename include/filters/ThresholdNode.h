#pragma once
#include "BaseNode.h"
#include <opencv2/opencv.hpp>

class ThresholdNode : public BaseNode {
public:
    cv::Mat inputImage;
    cv::Mat outputImage;

    int thresholdValue = 128;

    ThresholdNode(int id);
    void SetInput(const cv::Mat& image);
    void Process() override;
};
