#include "stdafx.h"
#include "ConfigParser.h"

#include "Utils.hpp"

/////////////////////////////////////////////////////////////////////////
void ConfigParser::ParseJSON(const std::string & jsonFile, Config & config)
{
  FILE* pFile = fopen(jsonFile.c_str(), "rb");
  char buffer[65536];
  FileReadStream is(pFile, buffer, sizeof(buffer));
  Document document;
  document.ParseStream<0, UTF8<>, FileReadStream>(is);
  config.HaarFile = document[HaarFile].GetString();
  config.NestedHaarFile = document[NestedHaarFile].GetString();
  config.IntrudersFolder = document[IntrudersFolder].GetString();
  config.DeviceId = document[DeviceId].GetInt();
  auto data = document[KnownFaces].GetArray();
  ParseKnownFaces(data, config);
}

/////////////////////////////////////////////////////////////////////////
void ConfigParser::ParseKnownFaces(GenericArray<false, GenericValue<UTF8<>>> data, Config & config)
{
  for(unsigned int i = 0; i < data.Size(); i++)
  {
    KnownFace face;
    face.Name = data[i][Name].GetString();
    Utility::getContentsInDir(data[i][TrainingImages].GetString(), face.TrainingImages);
    ParseSpeeches(data[i][Speeches].GetArray(), face.Speeches);
    config.KnownFaces.push_back(face);
  }
}

/////////////////////////////////////////////////////////////////////////
void ConfigParser::ParseSpeeches(GenericArray<false, GenericValue<UTF8<>>> data, std::set<std::string>& speeches)
{
  for(auto& v: data)
  {
    speeches.insert(v.GetString());
  }
}

/////////////////////////////////////////////////////////////////////////
ConfigParser::ConfigParser()
{
  // Disabled
}

/////////////////////////////////////////////////////////////////////////
void ConfigParser::Parse(const std::string & configFile, Config & config)
{
  ParseJSON(configFile, config);
}
