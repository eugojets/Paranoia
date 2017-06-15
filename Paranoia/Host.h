#pragma once
#include "stdafx.h"
#include "sapi.h"

#include <string>
#include <map>
#include <set>

#include "spdlog\spdlog.h"

typedef std::shared_ptr<spdlog::logger> Logger;

class Host
{
private:
  void InitializeVoice();
  std::map<std::string, std::set<std::string>> Speeches;
  std::set<std::string> PreviousPeople;
  ISpVoice* Voice;
  bool VoiceInitialized;
  Logger Logger;

public:
  Host();
  ~Host();
  void AddSpeech(const std::string& name, const std::string& speech);
  void ClearSpeech(const std::string& name);
  void Greet(const std::vector<std::string>& people);
};