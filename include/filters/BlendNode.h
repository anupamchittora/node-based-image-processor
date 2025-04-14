#pragma once
#include "BaseNode.h"
#include <opencv2/opencv.hpp>

enum class BlendMode {
    Normal,
    Multiply,
    Screen,
    Overlay,
    Difference
};

class BlendNode : public BaseNode {
public:
    cv::Mat inputA, inputB;
    cv::Mat outputImage;

    BlendMode mode = BlendMode::Normal;
    float opacity = 1.0f;

    BlendNode(int id);
    cv::Mat GetOutput() const override { return outputImage; }

    void SetInputA(const cv::Mat& img);
    void SetInputB(const cv::Mat& img);
    void Process() override;
};
