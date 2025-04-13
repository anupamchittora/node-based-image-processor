#pragma once
#include "BaseNode.h"
#include <opencv2/opencv.hpp>

class ImageInputNode : public BaseNode {
public:
    cv::Mat image;

    ImageInputNode(int id);
    void LoadImage(const std::string& path);
    void Process() override;
};
