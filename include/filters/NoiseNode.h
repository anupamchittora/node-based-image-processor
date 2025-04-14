#pragma once
#include "BaseNode.h"
#include <opencv2/opencv.hpp>

class NoiseNode : public BaseNode {
public:
    cv::Mat outputImage;

    int width = 256;
    int height = 256;
    float scale = 10.0f;
    int seed = 0;

    NoiseNode(int id);
    void Process() override;

    cv::Mat GenerateNoise();
};
