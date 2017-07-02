#pragma once
#include "stdafx.h"
#include <ctime>
#include <map>
#include <set>
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
  std::set<std::string> AuthorizedUsers;
  Time Delay;
  unsigned int MaxQueueSize;
  ImageSaver ImageSaver;
public:
  ImageQueueManager(std::string rootFolder, int maxSize, unsigned int delay);
  virtual ~ImageQueueManager();
  virtual void FacesRecognized(const std::vector<std::string>& names, const cv::Mat& frame);
  void AddAuthorizedUsers(const std::vector<std::string>& users);
  void Push(const std::string& name, const cv::Mat& data);
  void Flush();
};