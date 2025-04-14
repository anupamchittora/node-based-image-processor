#pragma once
#include "BaseNode.h"
#include "ImageInputNode.h"
#include "OutputNode.h"
#include "filters/GrayscaleNode.h"
#include "filters/BrightnessContrastNode.h"
#include "filters/ColorChannelSplitterNode.h"
#include "filters/BlurNode.h"
#include "filters/ThresholdNode.h"
#include "filters/EdgeDetectionNode.h"
#include "filters/BlendNode.h"
#include "filters/NoiseNode.h"

BaseNode* CreateNodeByName(const std::string& name, int id);
