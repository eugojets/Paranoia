#pragma once
#include "stdafx.h"

#include "ImageQueueManager.h"

/////////////////////////////////////////////////////////////////////////
bool ImageQueueManager::ShouldEnqueue(const std::string& name)
{
  return Utility::now() - ImageQueues[name].Back().timestamp > Delay;
}

/////////////////////////////////////////////////////////////////////////
ImageQueueManager::ImageQueueManager(unsigned int maxSize = 5, unsigned int delay = 30) :
  Delay((Time)delay),
  MaxQueueSize(maxSize)
{
}

/////////////////////////////////////////////////////////////////////////
ImageQueueManager::~ImageQueueManager()
{
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
  }
}

/////////////////////////////////////////////////////////////////////////
void ImageQueueManager::Flush()
{
}
