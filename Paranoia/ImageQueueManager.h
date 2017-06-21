#pragma once
#include "stdafx.h"
#include <ctime>
#include <map>
#include <string>

#include <opencv2/core/core.hpp>

#include "BoundedQueue.h"
#include "Utils.hpp"

struct ImageBuffer
{
  cv::Mat Image;
  Time timestamp;
};

class ImageQueueManager
{
private:
  bool ShouldEnqueue(const std::string& name);
  std::map<std::string, BoundedDeque<ImageBuffer>> ImageQueues;
  Time Delay;
  unsigned int MaxQueueSize;
public:
  ImageQueueManager(unsigned int maxSize, unsigned int delay);
  virtual ~ImageQueueManager();
  void Push(const std::string& name, const cv::Mat& data);
  void Flush();
};