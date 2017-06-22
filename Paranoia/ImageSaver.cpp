#pragma once
#include "stdafx.h"
#include "ImageSaver.h"

#include <future>
#include <filesystem>
#include <vector>

#include <opencv2/core/core.hpp>

#include "Utils.hpp"

namespace fs = std::experimental::filesystem;

/////////////////////////////////////////////////////////////////////////
void ImageSaver::GetTarget(const std::string& filename, std::string& target)
{
  fs::path file(filename + ".png");
  fs::path targetPath = RootFolder / file;
  target = targetPath.string();
}

/////////////////////////////////////////////////////////////////////////
ImageSaver::ImageSaver(std::string rootFolder) : RootFolder(rootFolder)
{
  Logger = spdlog::stdout_color_mt(Utility::Nameof(this));
}

/////////////////////////////////////////////////////////////////////////
ImageSaver::~ImageSaver()
{
}

/////////////////////////////////////////////////////////////////////////
void ImageSaver::SaveImage(const std::string& filename, const cv::Mat& image)
{
  std::string target;
  GetTarget(filename, target);
  bool success = imwrite(target, image);
  Logger->info("Saving image {0} {1}.", target, success ? "successful" : "failed");
}

/////////////////////////////////////////////////////////////////////////
void ImageSaver::SaveImageAsync(const std::string& filename, const cv::Mat& image)
{
  std::string target;
  GetTarget(filename, target);
  std::future<bool> success = std::async(cv::imwrite, target, image, std::vector<int>());
  Logger->info("Saving image {0} {1}.", target, success.get() ? "successful" : "failed");
}
