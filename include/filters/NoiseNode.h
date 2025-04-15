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
    cv::Mat GetOutput() const override {
        return outputImage;
    }
    void SetInput(const cv::Mat&) override {
        std::cerr << "[NoiseNode] Does not accept input.\n";
    }

    NoiseNode(int id);
    void Process() override;

    cv::Mat GenerateNoise();
};
