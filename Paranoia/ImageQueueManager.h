#pragma once
#include "stdafx.h"
#include <ctime>
#include <map>
#include <string>

#include <opencv2/core/core.hpp>

#include "BoundedDeque.h"
#include "IFacesRecognizedObserver.h"
#include "ImageSaver.h"
#include "Utils.hpp"

struct ImageBuffer
{
  cv::Mat Image;
  Time timestamp;
};

class ImageQueueManager : public IFacesRecognizedObserver
{
private:
  bool ShouldEnqueue(const std::string& name);
  std::map<std::string, BoundedDeque<ImageBuffer>> ImageQueues;
  Time Delay;
  unsigned int MaxQueueSize;
  ImageSaver ImageSaver;
public:
  ImageQueueManager(std::string rootFolder, int maxSize, unsigned int delay);
  virtual ~ImageQueueManager();
  virtual void FacesRecognized(std::vector<std::string> names, const cv::Mat& frame);
  void Push(const std::string& name, const cv::Mat& data);
  void Flush();
};