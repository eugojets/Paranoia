#pragma once
#include "stdafx.h"
#include <future>
#include <memory>
#include <thread>
#include <stdlib.h>
#include <time.h>

#include <spdlog/spdlog.h>

#include "FaceDetector.h"
#include "FaceRecognizer.h"
#include "GammaRamp.h"
#include "Host.h"
#include "VideoCaptureManager.h"

namespace spd = spdlog;

/////////////////////////////////////////////////////////////////////////
void AddSpeech(Host& host, const ConfigParser::Config& config)
{
  for(auto& face : config.KnownFaces)
  {
    for(auto& speech : face.Speeches)
    {
      host.AddSpeech(face.Name, speech);
    }
    host.AddSpeech(face.Name, "Hello " + face.Name + ", how is it going?");
  }
}

/////////////////////////////////////////////////////////////////////////
void ConfigureFaceRecognizer(FaceRecognizer& recognizer,
  VideoCaptureManager& videoCaptureManager,
  Host& host,
  const ConfigParser::Config& config)
{
  AddSpeech(host, config);
  videoCaptureManager.RegisterFrameObserver(&recognizer);
  recognizer.DisplayLiveFeed(false);

  auto OnRecognizeFaces = [&host](std::vector<std::string> people)
  {
    host.Greet(people);
  };

  recognizer.OnRecognizeFaces.connect(OnRecognizeFaces);
}

/////////////////////////////////////////////////////////////////////////
void ConfigureFaceDetector(FaceDetector& detector, VideoCaptureManager& videoCaptureManager, CGammaRamp& ramp)
{
  detector.DisplayLiveFeed(false);
  detector.ShowDetectedFaces(true);
  detector.SetColor(0, FaceDetector::Color::PURPLE);

  auto OnFaceDetected = [&ramp, &detector](FaceCount count)
  {
    if(ramp.GetCurrentBrightness() != CGammaRamp::NORMAL)
    {
      ramp.ResetBrightness();
    }
  };

  auto OnNoFaceDetected = [&ramp]()
  {
    if(ramp.GetCurrentBrightness() != CGammaRamp::DIMMEST)
    {
      ramp.SetBrightness(CGammaRamp::DIMMEST);
    }
  };

  detector.OnFaceDetected.connect(OnFaceDetected);
  detector.OnNoFaceDetected.connect(OnNoFaceDetected);
  videoCaptureManager.RegisterFrameObserver(&detector);
}

/////////////////////////////////////////////////////////////////////////
int main(int argc, const char *argv[])
{
  auto console = spd::stdout_color_mt("Main");
  console->set_level(spd::level::debug);
  if(argc < 2)
  {
    console->error("Need to provide path to config.json");
    return 1;
  }
  srand((uint)time(nullptr));

  std::string configFile = argv[1];
  ConfigParser::Config config;
  ConfigParser::Parse(configFile, config);
  Host host;
  VideoCaptureManager videoCaptureManager(config.DeviceId);

  // FaceRecognizer
  FaceRecognizer recognizer(config);
  ConfigureFaceRecognizer(recognizer, videoCaptureManager, host, config);

  // FaceDetector
  //FaceDetector detector(config);
  //CGammaRamp gammeRamp;
  //ConfigureFaceDetector(detector, videoCaptureManager, gammeRamp);

  // This starts video recording asynchronously
  std::future<void> videoCaptureTask = std::async(std::launch::async, &VideoCaptureManager::StartVideoCapture, &videoCaptureManager);

  return 0;
}

