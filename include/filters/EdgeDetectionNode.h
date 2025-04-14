#pragma once
#include "BaseNode.h"
#include <opencv2/opencv.hpp>

enum class EdgeMode {
    Sobel,
    Canny
};

class EdgeDetectionNode : public BaseNode {
public:
    cv::Mat inputImage;
    cv::Mat outputImage;

    EdgeMode mode = EdgeMode::Sobel;
    int kernelSize = 3;
    int cannyThreshold1 = 50;
    int cannyThreshold2 = 150;

    EdgeDetectionNode(int id);
    cv::Mat GetOutput() const override { return outputImage; }

    void SetInput(const cv::Mat& image);
    void Process() override;
};
