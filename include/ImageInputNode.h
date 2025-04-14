#pragma once
#include "BaseNode.h"
#include <opencv2/opencv.hpp>

class ImageInputNode : public BaseNode {
public:
    cv::Mat outputImage;
    std::string imagePath;

    ImageInputNode(int id);
    void SetImagePath(const std::string& path);
    void Process() override;
};
