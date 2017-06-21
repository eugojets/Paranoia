#pragma once

#include "stdafx.h"
#include <string>
#include <vector>
#include <set>

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <spdlog/spdlog.h>

using namespace rapidjson;

typedef unsigned int uint;

class ConfigParser
{
public:
  struct KnownFace
  {
    std::string Name;
    std::vector<std::string> TrainingImages;
    std::set<std::string> Speeches;
  };

  struct Config
  {
    std::string HaarFile;
    std::string NestedHaarFile;
    std::string IntrudersFolder;
    uint DeviceId;
    std::vector<KnownFace> KnownFaces;
  };

private:
  static void ParseJSON(const std::string& jsonFile, Config& config);
  static void ParseKnownFaces(GenericArray<false, GenericValue<UTF8<>>> data, Config& config);
  static void ParseSpeeches(GenericArray<false, GenericValue<UTF8<>>> data, std::set<std::string>& speeches);
  ConfigParser();

  static constexpr const char* HaarFile = "haarFile";
  static constexpr const char* NestedHaarFile = "nestedHaarFile";
  static constexpr const char* IntrudersFolder = "intrudersFolder";
  static constexpr const char* DeviceId = "deviceId";
  static constexpr const char* KnownFaces = "knownFaces";
  static constexpr const char* Name = "name";
  static constexpr const char* TrainingImages = "trainingImages";
  static constexpr const char* Speeches = "speeches";

public:
  static void Parse(const std::string& configFile, Config& config);
};

