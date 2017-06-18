#pragma once
#include "stdafx.h"
#include <opencv2\highgui\highgui.hpp>

class IFrameObserver
{
public:
  virtual void ProcessFrame(cv::Mat& frame) = 0;
};
