#pragma once
#include "BaseNode.h"
#include <opencv2/opencv.hpp>
#include <vector>

enum ChannelOutput { RED, GREEN, BLUE, ALPHA };  // 👈 Add this enum

class ColorChannelSplitterNode : public BaseNode {
public:
    cv::Mat inputImage;
    cv::Mat rChannel, gChannel, bChannel, aChannel;
    bool hasAlpha = false;

    ChannelOutput selectedOutput = RED;  // 👈 New member

    ColorChannelSplitterNode(int id);

    void SetInput(const cv::Mat& image);
    void Process() override;
    cv::Mat GetOutput() const override;  // 👈 Implement this
};
