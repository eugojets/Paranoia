#pragma once
#include "stdafx.h"
#include "ImageSaver.h"

#include <future>
#include <filesystem>
#include <iomanip>
#include <vector>

#include <opencv2/core/core.hpp>

#include "Utils.hpp"

namespace fs = std::experimental::filesystem;

/////////////////////////////////////////////////////////////////////////
void ImageSaver::GetTarget(const std::string& name, std::string& target)
{
  fs::path targetDir = RootFolder / name;
  BootstrapDir(name);
  std::string time;
  Utility::getTimeWithFormat("%d-%m-%Y_%H%M%S", time);
  fs::path file(time + ".png");
  fs::path targetPath = targetDir / file;
  target = targetPath.string();
}

/////////////////////////////////////////////////////////////////////////
void ImageSaver::BootstrapDir(const std::string& directory)
{
  fs::path target = RootFolder / directory;

  if(!fs::exists(target))
  {
    Logger->info("Creating folder for {0}", directory);
    if(!fs::create_directory(target))
    {
      Logger->warn("Failed to create folder for {0}", directory);
    }
  }
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
