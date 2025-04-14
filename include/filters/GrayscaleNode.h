// GrayscaleNode.h
#pragma once
#include "BaseNode.h"
#include <opencv2/opencv.hpp>

class GrayscaleNode : public BaseNode {
public:
    cv::Mat inputImage;
    cv::Mat outputImage;
    cv::Mat GetOutput() const override { return outputImage; }

    GrayscaleNode(int id);
    void SetInput(const cv::Mat& image);
    void Process() override;
};
