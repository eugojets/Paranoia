#pragma once
#include "stdafx.h"
#include <string>

#include <opencv2/highgui/highgui.hpp>
#include <spdlog/spdlog.h>

typedef std::shared_ptr<spdlog::logger> Logger;

class ImageSaver
{
private:
  Logger Logger;
  std::string RootFolder;
public:
  ImageSaver(std::string rootFolder);
  virtual ~ImageSaver();
  void SaveImage(std::string target, const cv::Mat& image);
};
