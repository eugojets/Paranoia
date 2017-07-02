#pragma once
#include "stdafx.h"
#include <filesystem>
#include <string>

#include <opencv2/highgui/highgui.hpp>
#include <spdlog/spdlog.h>

namespace fs = std::experimental::filesystem;

typedef std::shared_ptr<spdlog::logger> Logger;

class ImageSaver
{
private:
  void GetTarget(const std::string& name, std::string& target);
  void BootstrapDir(const std::string& folder);
  Logger Logger;
  fs::path RootFolder;
  ImageSaver();
public:
  ImageSaver(std::string rootFolder);
  virtual ~ImageSaver();
  void SaveImage(const std::string& filename, const cv::Mat& image);
  void SaveImageAsync(const std::string& filename, const cv::Mat& image);
};
