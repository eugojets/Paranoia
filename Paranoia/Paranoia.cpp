#pragma once
#include "stdafx.h"
#include <future>
#include <memory>
#include <thread>
#include <stdlib.h>
#include <time.h>

#include <spdlog\spdlog.h>

#include "FaceDetector.h"
#include "FaceRecognizer.h"
#include "Host.h"
#include "GammaRamp.h"

namespace spd = spdlog;

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

int main(int argc, const char *argv[])
{
  auto console = spd::stdout_color_mt("Main");
  console->set_level(spd::level::debug);
  if(argc < 2)
  {
    console->error("Need to provide path to config.json");
    return 1;
  }
  srand(time(nullptr));

  std::string configFile = argv[1];
  ConfigParser::Config config;
  ConfigParser::Parse(configFile, config);
  Host host;

  AddSpeech(host, config);

  FaceRecognizer recognizer(config);
  std::future<void> facialRecognitionTask = std::async(std::launch::async, &FaceRecognizer::StartFacialRecognition, &recognizer);
  //recognizer.DisplayLiveFeed(true);

  auto OnRecognizeFaces = [&host](std::vector<std::string> people)
  {
    host.Greet(people);
  };

  recognizer.OnRecognizeFaces.connect(OnRecognizeFaces);

  //FaceDetector detector(config);
  //CGammaRamp GammaRamp;

  //std::future<void> fut = std::async(std::launch::async, &FaceDetector::StartFaceDetection, &detector);

  //detector.DisplayLiveFeed(true);
  //detector.ShowDetectedFaces(true);
  //detector.SetColor(0, FaceDetector::Color::PURPLE);

  //auto OnFaceDetected = [&GammaRamp, &detector](FaceCount count)
  //{
  //  if(GammaRamp.GetCurrentBrightness() != CGammaRamp::NORMAL)
  //  {
  //    GammaRamp.ResetBrightness();
  //  }
  //};

  //auto OnNoFaceDetected = [&GammaRamp]()
  //{
  //  if(GammaRamp.GetCurrentBrightness() != CGammaRamp::DIMMEST)
  //  {
  //    GammaRamp.SetBrightness(CGammaRamp::DIMMEST);
  //  }
  //};

  //detector.OnFaceDetected.connect(OnFaceDetected);
  //detector.OnNoFaceDetected.connect(OnNoFaceDetected);

  return 0;
}

