#pragma once
#include "stdafx.h"
#include "VideoCaptureManager.h"

#include "Utils.hpp"

/////////////////////////////////////////////////////////////////////////
void VideoCaptureManager::Loop()
{
  cv::Mat frame;
  while(IsCapturingVideo)
  {
    VideoCapture >> frame;
    for(auto& observer : Observers)
    {
      observer->ProcessFrame(frame.clone());
    }

    // If user hits an escape key
    if(EscapeKeys.count(cv::waitKey(10)))
    {
      IsCapturingVideo = false;
    }
  }
}

/////////////////////////////////////////////////////////////////////////
bool VideoCaptureManager::InitializeCamera()
{
  VideoCapture.open(DeviceId);
  bool open = VideoCapture.isOpened();
  if(open)
  {
    Logger->info("Successfully opened camera");
  }
  else
  {
    Logger->error("Could not open camera with device id {0}", DeviceId);
  }
  return open;
}

/////////////////////////////////////////////////////////////////////////
VideoCaptureManager::VideoCaptureManager(int deviceId) :
  DeviceId(deviceId),
  IsCapturingVideo(false)
{
  Logger = spdlog::stdout_color_mt(Utility::Nameof(this));
  EscapeKeys.insert(27); // Esc is the default escape key
}

/////////////////////////////////////////////////////////////////////////
VideoCaptureManager::~VideoCaptureManager()
{
}

/////////////////////////////////////////////////////////////////////////
void VideoCaptureManager::StartVideoCapture()
{
  if(InitializeCamera())
  {
    Logger->info("Starting video capture");
    IsCapturingVideo = true;
    Loop();
  }
}

/////////////////////////////////////////////////////////////////////////
void VideoCaptureManager::StopVideoCapture()
{
  IsCapturingVideo = false;
}

/////////////////////////////////////////////////////////////////////////
void VideoCaptureManager::AddEscapeKey(int key)
{
  Locker lock(EscapeKeyLock);
  EscapeKeys.insert(key);
}

/////////////////////////////////////////////////////////////////////////
void VideoCaptureManager::SetEscapeKeys(std::set<int> keys)
{
  Locker lock(EscapeKeyLock);
  EscapeKeys = keys;
}

/////////////////////////////////////////////////////////////////////////
void VideoCaptureManager::RegisterFrameObserver(IFrameObserver* observer)
{
  Observers.insert(observer);
}
