#pragma once
#include "stdafx.h"
#include <set>

#include <opencv2\highgui\highgui.hpp>
#include <spdlog\spdlog.h>

typedef std::shared_ptr<spdlog::logger> Logger;


class VideoCaptureManager
{
private:
  void Loop();
  bool InitializeCamera();

  cv::VideoCapture VideoCapture;
  bool IsCapturingVideo;
  int DeviceId;
  std::set<void(*)(cv::Mat&)> Callbacks;
  Logger Logger;

public:
  VideoCaptureManager(int deviceId);
  ~VideoCaptureManager();
  void StartVideoCapture();
  void StopVideoCapture();
  void RegisterForFrame(void(*cb)(cv::Mat&));
};
