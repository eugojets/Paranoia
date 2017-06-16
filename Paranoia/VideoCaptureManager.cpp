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
    for(auto& cb : Callbacks)
    {
      cb(frame.clone());
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
void VideoCaptureManager::RegisterForFrame(void(*cb)(cv::Mat&))
{
  Callbacks.insert(*cb);
}
