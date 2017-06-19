#pragma once
#include "stdafx.h"
#include <set>

#include <opencv2/highgui/highgui.hpp>
#include <spdlog/spdlog.h>

#include "IFrameObserver.h"
#include "Locker.h"

typedef std::shared_ptr<spdlog::logger> Logger;

class VideoCaptureManager
{
private:
  void Loop();
  bool InitializeCamera();

  cv::VideoCapture VideoCapture;
  bool IsCapturingVideo;
  int DeviceId;
  std::set<int> EscapeKeys;
  std::mutex EscapeKeyLock;
  std::set<IFrameObserver*> Observers;
  Logger Logger;

public:
  VideoCaptureManager(int deviceId);
  virtual ~VideoCaptureManager();
  void StartVideoCapture();
  void StopVideoCapture();
  void AddEscapeKey(int key);
  void SetEscapeKeys(std::set<int> keys);
  void RegisterFrameObserver(IFrameObserver* observer);
};
