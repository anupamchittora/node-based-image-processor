#pragma once
#include "BaseNode.h"
#include <opencv2/opencv.hpp>
#include <vector>

class ColorChannelSplitterNode : public BaseNode {
public:
    cv::Mat inputImage;
    cv::Mat rChannel, gChannel, bChannel, aChannel;
    bool hasAlpha = false;

    ColorChannelSplitterNode(int id);

    void SetInput(const cv::Mat& image);
    cv::Mat GetOutput() const override {
        return rChannel;  // Could allow toggling later
    }

    void Process() override;
};
