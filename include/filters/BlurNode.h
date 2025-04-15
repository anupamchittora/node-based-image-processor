#pragma once
#include "BaseNode.h"
#include <opencv2/opencv.hpp>

class BlurNode : public BaseNode {
public:
    cv::Mat inputImage;
    cv::Mat outputImage;

    int radius = 3;  // Default blur radius
    cv::Mat GetOutput() const override {
        return outputImage;
    }


    BlurNode(int id);
    void SetInput(const cv::Mat& image);
    void Process() override;
};
