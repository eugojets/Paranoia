#pragma once
#include "stdafx.h"
#include <string>
#include <vector>

#include <opencv2/highgui/highgui.hpp>

class IFacesRecognizedObserver
{
public:
  virtual void FacesRecognized(const std::vector<std::string>& names, const cv::Mat& frame) = 0;
};

