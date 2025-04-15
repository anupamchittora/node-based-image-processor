#pragma once
#include "BaseNode.h"
#include <opencv2/opencv.hpp>
#include <GL/glew.h>

class NoiseNode : public BaseNode {
public:
    cv::Mat outputImage;
    int width = 256;
    int height = 256;
    float scale = 10.0f;
    int seed = 0;

    GLuint textureID = 0;

    NoiseNode(int id);
    void Process() override;
    void CreateTexture();
    cv::Mat GetOutput() const override { return outputImage; }
    ~NoiseNode();
};
