#pragma once
#include "BaseNode.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

class ConvolutionNode : public BaseNode {
public:
    cv::Mat inputImage;
    cv::Mat outputImage;

    std::vector<float> kernel;
    int kernelSize = 3; // 3 or 5

    ConvolutionNode(int id);

    void SetInput(const cv::Mat& image);
    void Process() override;
    void SetPreset(const std::string& name);
};
