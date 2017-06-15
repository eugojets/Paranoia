#pragma once
#include "stdafx.h"
#include "FaceDetector.h"

#include "Utils.hpp"

std::unordered_map<FaceDetector::Color, Scalar> FaceDetector::ColorMap =
{
  {Color::BLUE, Scalar(255,0,0)},
  {Color::LIGHTBLUE, Scalar(255,128,0)},
  {Color::TURQUOISE,  Scalar(255,255,0)},
  {Color::GREEN, Scalar(0,255,0)},
  {Color::ORANGE,  Scalar(0,128,255)},
  {Color::YELLOW, Scalar(0,255,255)},
  {Color::RED, Scalar(0,0,255)},
  {Color::PURPLE, Scalar(255,0,255)},
};

/////////////////////////////////////////////////////////////////////////
bool FaceDetector::LoadCascades(const std::string& haarFile, const std::string& nestedHaarFile)
{
  CascadesLoaded = true;
  if(!NestedCascade.load(nestedHaarFile))
  {
    Logger->error("Could not load classifier cascade for nested objects");
    CascadesLoaded = false;
  }
  if(!Cascade.load(haarFile))
  {
    Logger->error("Could not load classifier cascade");
    CascadesLoaded = false;
  }
  return CascadesLoaded;
}

/////////////////////////////////////////////////////////////////////////
bool FaceDetector::InitializeCamera()
{
  VideoCapture.open(0);
  if(VideoCapture.isOpened())
  {
    Logger->info("Successfully opened camera");
  }
  else
  {
    Logger->error("Could not open camera");
  }
  return VideoCapture.isOpened();
}

/////////////////////////////////////////////////////////////////////////
void FaceDetector::Loop()
{
  while(IsDetectingFaces)
  {
    Mat frame;
    VideoCapture >> frame;
    if(frame.empty())
    {
      Logger->error("Frame is empty");
      break;
    }
    DetectAndDraw(frame.clone(), 1, false);
    if(EscapeKeys.count(waitKey(10)))
    {
      Logger->debug("Exiting loop");
      EndFaceDetection();
    }
  }
}

/////////////////////////////////////////////////////////////////////////
void FaceDetector::CallSignals()
{
  if(CurrentFrame > 0)
  {
    OnFaceDetected(CurrentFrame);
  }
  else
  {
    OnNoFaceDetected();
  }
  if(CurrentFrame != PrevFrame)
  {
    OnFaceCountChanged(CurrentFrame, PrevFrame);
  }
}

/////////////////////////////////////////////////////////////////////////
void FaceDetector::DetectAndDraw(Mat& img, double scale, bool tryFlip)
{
  std::vector<Rect> faces, faces2;
  Mat gray, smallImg;

  // Detect
  FaceDetector::Detect(img, gray, smallImg, faces, scale, tryFlip);

  // Callbacks
  PrevFrame = FaceCount(CurrentFrame);
  CurrentFrame = faces.size();
  CallSignals();

  // Draw
  FaceDetector::Draw(faces, img, smallImg, scale);
}

/////////////////////////////////////////////////////////////////////////
void FaceDetector::Detect(Mat& img, Mat& gray, Mat& smallImg, std::vector<Rect>& faces, double scale, bool tryFlip)
{
  double t = 0;
  std::vector<Rect> faces2;
  cvtColor(img, gray, COLOR_BGR2GRAY);
  double fx = 1 / scale;
  resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR);
  equalizeHist(smallImg, smallImg);

  t = (double)getTickCount();
  Cascade.detectMultiScale(smallImg, faces,
    1.1, 2, 0
    //|CASCADE_FIND_BIGGEST_OBJECT
    //|CASCADE_DO_ROUGH_SEARCH
    | CASCADE_SCALE_IMAGE,
    Size(30, 30));
  if(tryFlip)
  {
    flip(smallImg, smallImg, 1);
    Cascade.detectMultiScale(smallImg, faces2,
      1.1, 2, 0
      //|CASCADE_FIND_BIGGEST_OBJECT
      //|CASCADE_DO_ROUGH_SEARCH
      | CASCADE_SCALE_IMAGE,
      Size(30, 30));
    for(std::vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); ++r)
    {
      faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
    }
  }
  t = (double)getTickCount() - t;
  DetectionTime = t * 1000 / getTickFrequency();

  PrevFrame = FaceCount(CurrentFrame);
  CurrentFrame = faces.size();
}

/////////////////////////////////////////////////////////////////////////
void FaceDetector::Draw(const std::vector<Rect> faces, Mat img, Mat smallImg, double scale)
{
  const static Scalar colors[] =
  {
    Scalar(255,0,0), // Blue
    Scalar(255,128,0), // LightBlue
    Scalar(255,255,0), // Turquoise
    Scalar(0,255,0), // Green
    Scalar(0,128,255), // Orange
    Scalar(0,255,255), // Yellow
    Scalar(0,0,255), // Red
    Scalar(255,0,255) // Purple
  };

  for(size_t i = 0; i < faces.size() && ShowFaceDetection; i++)
  {
    Rect r = faces[i];
    Mat smallImgROI;
    std::vector<Rect> nestedObjects;
    Point center;
    Scalar color = ColorMap[FaceDetectionColors[i % FaceDetectionColors.size()]];
    int radius;

    double aspect_ratio = (double)r.width / r.height;
    if(0.75 < aspect_ratio && aspect_ratio < 1.3)
    {
      center.x = cvRound((r.x + r.width*0.5)*scale);
      center.y = cvRound((r.y + r.height*0.5)*scale);
      radius = cvRound((r.width + r.height)*0.25*scale);
      circle(img, center, radius, color, 3, 8, 0);
    }
    else
      rectangle(img, cvPoint(cvRound(r.x*scale), cvRound(r.y*scale)),
        cvPoint(cvRound((r.x + r.width - 1)*scale), cvRound((r.y + r.height - 1)*scale)),
        color, 3, 8, 0);
    if(NestedCascade.empty())
      continue;
    smallImgROI = smallImg(r);
    NestedCascade.detectMultiScale(smallImgROI, nestedObjects,
      1.1, 2, 0
      //|CASCADE_FIND_BIGGEST_OBJECT
      //|CASCADE_DO_ROUGH_SEARCH
      //|CASCADE_DO_CANNY_PRUNING
      | CASCADE_SCALE_IMAGE,
      Size(30, 30));
    for(size_t j = 0; j < nestedObjects.size() && ShowEyeDetection; j++)
    {
      Rect nr = nestedObjects[j];
      center.x = cvRound((r.x + nr.x + nr.width*0.5)*scale);
      center.y = cvRound((r.y + nr.y + nr.height*0.5)*scale);
      radius = cvRound((nr.width + nr.height)*0.25*scale);
      circle(img, center, radius, color, 3, 8, 0);
    }
  }
  if(ShouldDisplayLiveFeed)
  {
    cv::imshow("result", img);
  }
}

/////////////////////////////////////////////////////////////////////////
FaceDetector::FaceDetector(const ConfigParser::Config& config) :
  CascadesLoaded(false),
  IsDetectingFaces(false),
  DetectionTime(-1),
  ShouldDisplayLiveFeed(false),
  ShowEyeDetection(false),
  PrevFrame(0),
  CurrentFrame(0)
{
  EscapeKeys.insert(27); // Esc is the default escape key
  LoadCascades(config.HaarFile, config.NestedHaarFile);
  Logger = spdlog::stdout_color_mt(Utility::Nameof(this));

  FaceDetectionColors = std::vector<Color>(8, Color::BLUE);
}

/////////////////////////////////////////////////////////////////////////
FaceDetector::~FaceDetector()
{
  VideoCapture.release();
}

/////////////////////////////////////////////////////////////////////////
void FaceDetector::StartFaceDetection()
{
  if(CascadesLoaded)
  {
    if(InitializeCamera())
    {
      Logger->info("Starting Face Detection");
      IsDetectingFaces = true;
      Loop();
    }
  }
}

/////////////////////////////////////////////////////////////////////////
void FaceDetector::EndFaceDetection()
{
  IsDetectingFaces = false;
}

/////////////////////////////////////////////////////////////////////////
void FaceDetector::DisplayLiveFeed(bool display)
{
  ShouldDisplayLiveFeed = display;
}

/////////////////////////////////////////////////////////////////////////
void FaceDetector::AddEscapeKey(int key)
{
  Locker lock(EscapeKeyLock);
  EscapeKeys.insert(key);
}

/////////////////////////////////////////////////////////////////////////
void FaceDetector::SetEscapeKeys(std::set<int> keys)
{
  Locker lock(EscapeKeyLock);
  EscapeKeys = keys;
}

/////////////////////////////////////////////////////////////////////////
std::set<int> FaceDetector::GetEscapeKeys()
{
  Locker lock(EscapeKeyLock);
  return EscapeKeys;
}

/////////////////////////////////////////////////////////////////////////
void FaceDetector::ShowDetectedEyes(bool show)
{
  ShowEyeDetection = show;
}

/////////////////////////////////////////////////////////////////////////
void FaceDetector::ShowDetectedFaces(bool show)
{
  ShowFaceDetection = show;
}

/////////////////////////////////////////////////////////////////////////
void FaceDetector::SetColor(int pos, Color c)
{
  if(pos > FaceDetectionColors.size())
  {
    Logger->warn("Maximum number of colors supported is 8");
    Logger->warn("Face {0} will be using the same color as face {1}", pos, pos % 8);
  }
  else
  {
    FaceDetectionColors[pos] = c;
  }
}

/////////////////////////////////////////////////////////////////////////
bool FaceDetector::FaceDetected()
{
  return CurrentFrame > 0;
}

/////////////////////////////////////////////////////////////////////////
double FaceDetector::GetDetectionTime()
{
  return DetectionTime;
}
