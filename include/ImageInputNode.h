#pragma once
#include "BaseNode.h"
#include <opencv2/opencv.hpp>
#include <GL/glew.h> 
class ImageInputNode : public BaseNode {
public:
    cv::Mat outputImage;
    std::string imagePath;
    GLuint textureID = 0;

    ImageInputNode(int id);
    void SetImagePath(const std::string& path);
    void Process() override;
    void CreateTexture();
};
