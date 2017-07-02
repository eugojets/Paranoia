#pragma once
#include "stdafx.h"

#include "ImageQueueManager.h"

/////////////////////////////////////////////////////////////////////////
bool ImageQueueManager::ShouldEnqueue(const std::string& name)
{
  return AuthorizedUsers.count(name) == 0 && 
         Utility::now() - ImageQueues[name].Back().timestamp > Delay;
}

/////////////////////////////////////////////////////////////////////////
ImageQueueManager::ImageQueueManager(std::string rootFolder, int maxSize = 5, unsigned int delay = 30) :
  Delay((Time)delay),
  MaxQueueSize(maxSize),
  ImageSaver(rootFolder)
{
}

/////////////////////////////////////////////////////////////////////////
ImageQueueManager::~ImageQueueManager()
{
}

/////////////////////////////////////////////////////////////////////////
void ImageQueueManager::FacesRecognized(const std::vector<std::string>& names, const cv::Mat& frame)
{
  for(auto& name : names)
  {
    Push(name, frame);
  }
}

/////////////////////////////////////////////////////////////////////////
void ImageQueueManager::AddAuthorizedUsers(const std::vector<std::string>& users)
{
  for(auto& user : users)
  {
    AuthorizedUsers.insert(user);
  }
}

/////////////////////////////////////////////////////////////////////////
void ImageQueueManager::Push(const std::string& name, const cv::Mat& data)
{
  // Add a new BoundedQueue if person doesn't exist
  if(!ImageQueues.count(name))
  {
    ImageQueues[name] = BoundedDeque<ImageBuffer>(MaxQueueSize);
  }

  if(ShouldEnqueue(name))
  {
    ImageBuffer buffer =
    {
      data.clone(),
      Utility::now(),
    };
    ImageQueues[name].Push(buffer);
    ImageSaver.SaveImageAsync(name, buffer.Image);
  }
}

/////////////////////////////////////////////////////////////////////////
void ImageQueueManager::Flush()
{
}
