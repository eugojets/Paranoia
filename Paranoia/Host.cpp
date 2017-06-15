#pragma once
#include "stdafx.h"
#include "Host.h"

#include "Utils.hpp"

/////////////////////////////////////////////////////////////////////////
void Host::InitializeVoice()
{
  if(FAILED(::CoInitialize(nullptr)))
  {
    Logger->error("Failed to initialize voice");
  }
  else
  {
    HRESULT hr = CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, (void **)&Voice);
    VoiceInitialized = SUCCEEDED(hr);
  }
}

/////////////////////////////////////////////////////////////////////////
Host::Host() :
  VoiceInitialized(false),
  Voice(nullptr)
{
  Logger = spdlog::stdout_color_mt(Utility::Nameof(this));
  InitializeVoice();
}

/////////////////////////////////////////////////////////////////////////
Host::~Host()
{
  if(Voice)
  {
    Voice->Release();
    Voice = nullptr;
  }
  ::CoUninitialize();
}

/////////////////////////////////////////////////////////////////////////
void Host::AddSpeech(const std::string& name, const std::string& speech)
{
  if(!Speeches.count(name))
  {
    Speeches[name] = std::set<std::string>();
  }
  Speeches[name].insert(speech);
}

/////////////////////////////////////////////////////////////////////////
void Host::ClearSpeech(const std::string & name)
{
  if(Speeches.count(name))
  {
    Speeches[name].clear();
  }
}

/////////////////////////////////////////////////////////////////////////
void Host::Greet(const std::vector<std::string>& people)
{
  for(auto& name : people)
  {
    // if this person is not in the last frame, greet them
    if(!PreviousPeople.count(name))
    {
      if(Speeches.count(name))
      {
        std::set<std::string> allSpeeches = Speeches[name];
        std::set<std::string>::const_iterator it(Speeches[name].begin());
        int r = rand() % Speeches[name].size();
        std::advance(it, r);

        std::wstring wstr;
        Utility::stringToWstring(*it, wstr);
        if(FAILED(Voice->Speak(wstr.c_str(), SPF_IS_XML, nullptr)))
        {
          Logger->error("Failed to greet {0} with {1}", name, *it);
        }
      }
      else
      {
        Logger->warn("Tried to greet {0}, but {1} has no set speeches", name, name);
      }
    }
  }
  PreviousPeople.clear();
  PreviousPeople = std::set<std::string>(people.begin(), people.end());
}
