#pragma once
#include "stdafx.h"
#include <string>
#include <vector>
#include <map>

#include <boost\signals2.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/face.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <spdlog\spdlog.h>

#include "ConfigParser.h"
#include "Utils.hpp"

using boost::signals2::signal;
using namespace rapidjson;

typedef std::shared_ptr<spdlog::logger> Logger;
typedef unsigned int uint;

class FaceRecognizer
{
private:
  void LoadData(const std::vector<ConfigParser::KnownFace>& faces, std::vector<cv::Mat>& images, std::vector<int>& labels);
  void Loop();
  void GetFacesFromFrame(cv::Mat& frame, cv::Mat& gray, std::vector<cv::Rect_<int>>& faces);
  void ProcessFace(const cv::Mat& gray, const cv::Rect& face, cv::Mat original, std::string& prediction);
  void DrawPredictions(const cv::Rect& face, cv::Mat original, const std::string& prediction);

  std::unordered_map<int, std::string> People;
  cv::Ptr<cv::face::FaceRecognizer> FaceRecognitionModel;
  cv::CascadeClassifier Cascade;
  cv::VideoCapture VideoCapture;
  unsigned int DeviceId;
  unsigned int Width;
  unsigned int Height;
  Logger Logger;
  bool IsRecognizingFaces;
  bool ShouldDisplayLiveFeed;


public:
  FaceRecognizer(const ConfigParser::Config& config);
  virtual ~FaceRecognizer();
  void StartFacialRecognition();
  void StopFacialRecognition();
  void DisplayLiveFeed(bool display);
  void ProcessFrame(cv::Mat& frame);

  signal<void(std::vector<std::string>)> OnRecognizeFaces;
};
