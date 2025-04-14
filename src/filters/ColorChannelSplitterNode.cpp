#include "filters/ColorChannelSplitterNode.h"
#include <opencv2/opencv.hpp>
#include <iostream>

ColorChannelSplitterNode::ColorChannelSplitterNode(int id)
    : BaseNode(id, "Channel Splitter") {
}

void ColorChannelSplitterNode::SetInput(const cv::Mat& image) {
    inputImage = image;
    Process();
}

void ColorChannelSplitterNode::Process() {
    if (inputImage.empty()) return;

    std::vector<cv::Mat> channels;
    cv::split(inputImage, channels);

    rChannel = channels[2];  // OpenCV uses BGR by default
    gChannel = channels[1];
    bChannel = channels[0];

    if (channels.size() == 4) {
        hasAlpha = true;
        aChannel = channels[3];
    }
    else {
        hasAlpha = false;
        aChannel.release();
    }

    std::cout << "[ColorChannelSplitterNode] Channels split (R/G/B";
    if (hasAlpha) std::cout << "/A";
    std::cout << ")\n";
}
