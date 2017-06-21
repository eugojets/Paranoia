#pragma once
#include "stdafx.h"
#include "FaceRecognizer.h"

#include <future>

/////////////////////////////////////////////////////////////////////////
void FaceRecognizer::LoadData(const std::vector<ConfigParser::KnownFace>& faces, std::vector<cv::Mat>& images, std::vector<int>& labels)
{
  for(int i = 0; i < faces.size(); i++)
  {
    auto face = faces[i];
    for each(std::string path in face.TrainingImages)
    {
      images.push_back(cv::imread(path, 0));
      labels.push_back(i);
    }
    People[i] = face.Name;
  }
}

/////////////////////////////////////////////////////////////////////////
void FaceRecognizer::GetFacesFromFrame(cv::Mat& original, cv::Mat& gray, std::vector<cv::Rect_<int>>& faces)
{
  cvtColor(original, gray, CV_BGR2GRAY);
  Cascade.detectMultiScale(gray, faces);
}

/////////////////////////////////////////////////////////////////////////
void FaceRecognizer::ProcessFace(const cv::Mat& gray, const cv::Rect& face, cv::Mat original, std::string& prediction)
{
  cv::Mat grayScaleFace = gray(face);
  // Resizing the face
  cv::Mat faceResized;
  cv::resize(grayScaleFace, faceResized, cv::Size(Width, Height), 1.0, 1.0, cv::INTER_CUBIC);

  // Predict
  int predict = FaceRecognitionModel->predict(faceResized);
  prediction = People[predict];
  Logger->debug("Prediction: {0}", prediction);

  DrawPredictions(face, original, prediction);
}

/////////////////////////////////////////////////////////////////////////
void FaceRecognizer::DrawPredictions(const cv::Rect& face, cv::Mat original, const std::string& prediction)
{
  if(ShouldDisplayLiveFeed)
  {
    // Draw a green rectangle around the detected face:
    rectangle(original, face, CV_RGB(0, 255, 0), 1);
    // Create the text we will annotate the box with:
    std::string text = "Prediction = " + prediction;

    // Calculate the position for annotated text
    int posX = std::max(face.tl().x - 10, 0);
    int posY = std::max(face.tl().y - 10, 0);
    putText(original, text, cv::Point(posX, posY), cv::FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 255, 0), 2);
  }
}

/////////////////////////////////////////////////////////////////////////
FaceRecognizer::FaceRecognizer(const ConfigParser::Config& Config) :
  ShouldDisplayLiveFeed(false)
{
  Logger = spdlog::stdout_color_mt(Utility::Nameof(this));
  People[-1] = "Unknown";

  std::vector<cv::Mat> images;
  std::vector<int> labels;

  // Read the config file and prep the data
  LoadData(Config.KnownFaces, images, labels);

  // Images need to be cropped later
  Width = images[0].cols;
  Height = images[0].rows;

  // Train the model
  FaceRecognitionModel = cv::face::createFisherFaceRecognizer();
  FaceRecognitionModel->train(images, labels);
  Cascade.load(Config.HaarFile);

  Logger->info("Done Training Model");
}

/////////////////////////////////////////////////////////////////////////
FaceRecognizer::~FaceRecognizer()
{
}

/////////////////////////////////////////////////////////////////////////
void FaceRecognizer::DisplayLiveFeed(bool display)
{
  ShouldDisplayLiveFeed = display;
}

/////////////////////////////////////////////////////////////////////////
void FaceRecognizer::ProcessFrame(cv::Mat& frame)
{
  cv::Mat gray;
  std::vector<cv::Rect_<int>> faces;
  std::vector<std::string> predictions;
  GetFacesFromFrame(frame, gray, faces);

  // Process each face
  for(int i = 0; i < faces.size(); i++)
  {
    std::string name;
    ProcessFace(gray, faces[i], frame, name);
    predictions.push_back(name);
  }

  for(auto& observer : Observers)
  {
    //std::future<void> videoCaptureTask = std::async(std::launch::async, &VideoCaptureManager::StartVideoCapture, &videoCaptureManager);
    std::async(std::launch::async, &IFacesRecognizedObserver::FacesRecognized, &observer, predictions, frame.clone()); 
  }

  // Callback
  OnRecognizeFaces(predictions);

  // Display the faces
  if(ShouldDisplayLiveFeed)
  {
    imshow("FaceRecognizer", frame);
  }
}

/////////////////////////////////////////////////////////////////////////
void FaceRecognizer::RegisterFacesRecognizedObserver(IFacesRecognizedObserver* observer)
{
  Observers.insert(observer);
}
