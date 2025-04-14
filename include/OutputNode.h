#pragma once
#include "BaseNode.h"
#include <opencv2/opencv.hpp>

class OutputNode : public BaseNode {
public:
    cv::Mat inputImage;
    unsigned int textureID = 0;

    OutputNode(int id);
    void SetInput(const cv::Mat& image);
    void Process() override;
    void CreateTexture();
    void FreeTexture();
};
