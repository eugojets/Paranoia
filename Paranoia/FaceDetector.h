#pragma once
#include "stdafx.h"
#include <set>
#include <map>
#include <vector>

#include <boost\signals2.hpp>
#include <opencv2\opencv.hpp>
#include <spdlog\spdlog.h>

#include "Locker.h"
#include "ConfigParser.h"

using boost::signals2::signal;
using namespace::cv;

typedef unsigned int FaceCount;
typedef std::shared_ptr<spdlog::logger> Logger;

class FaceDetector
{
public:
  enum Color
  {
    BLUE,
    LIGHTBLUE,
    TURQUOISE,
    GREEN,
    ORANGE,
    YELLOW,
    RED,
    PURPLE
  };

private:
  bool LoadCascades(const std::string& haarFile, const std::string& nestedHaarFile);
  bool InitializeCamera();
  void Loop();
  void CallSignals();
  void DetectAndDraw(Mat& img, double scale, bool tryFlip);
  void Detect(Mat& img, Mat& gray, Mat& smallImg, std::vector<Rect>& faces, double scale, bool tryFlip);
  void Draw(const std::vector<Rect> faces, Mat img, Mat smallImg, double scale);

  std::atomic<bool> ShouldDisplayLiveFeed;
  std::atomic<bool> ShowEyeDetection;
  std::atomic<bool> ShowFaceDetection;
  std::atomic<bool> IsDetectingFaces;
  std::atomic<bool> CascadesLoaded;
  std::atomic<double> DetectionTime;
  std::atomic<FaceCount> PrevFrame;
  std::atomic<FaceCount> CurrentFrame;
  std::mutex EscapeKeyLock;
  std::set<int> EscapeKeys;
  std::vector<Color> FaceDetectionColors;
  static std::unordered_map<Color, Scalar> ColorMap;
  VideoCapture VideoCapture;
  CascadeClassifier Cascade;
  CascadeClassifier NestedCascade;
  Logger Logger;

public:
  FaceDetector(const ConfigParser::Config& config);
  virtual ~FaceDetector();
  void StartFaceDetection(); // Blocking call, so use async if you want to proceed with operations
  void EndFaceDetection();
  void DisplayLiveFeed(bool display);
  void AddEscapeKey(int key);
  void SetEscapeKeys(std::set<int> keys);
  std::set<int> GetEscapeKeys();
  void ShowDetectedEyes(bool detect);
  void ShowDetectedFaces(bool show);
  void SetColor(int pos, Color c);
  bool FaceDetected();
  double GetDetectionTime();

  signal<void()> OnNoFaceDetected;
  signal<void(FaceCount)> OnFaceDetected;
  signal<void(FaceCount, FaceCount)> OnFaceCountChanged;
};