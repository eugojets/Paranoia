#pragma once
#include "stdafx.h"
#include "Utils.hpp"

#include <iomanip>
#include <sstream>

namespace Utility
{
  /////////////////////////////////////////////////////////////////////////
  std::vector<std::string> split(const std::string &s, char delim)
  {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
  }

  /////////////////////////////////////////////////////////////////////////
  void getContentsInDir(std::string dir, std::vector<std::string>& contents)
  {
    HANDLE hFind;
    WIN32_FIND_DATA data;
    std::string temp = dir + "/*";
    std::wstring wstr;
    stringToWstring(temp, wstr);
    hFind = FindFirstFile(wstr.c_str(), &data);
    if(hFind != INVALID_HANDLE_VALUE)
    {
      do
      {
        std::wstring wsFileName (data.cFileName);
        std::string fileName = std::string(wsFileName.begin(), wsFileName.end());
        // have . or .. files on Windows
        if(fileName != "." && fileName != "..")
        {
          contents.push_back(dir + "\\" + fileName);
        }
      } while(FindNextFile(hFind, &data));
      FindClose(hFind);
    }
  }

  /////////////////////////////////////////////////////////////////////////
  void stringToWstring(const std::string& inStr, std::wstring& outStr)
  {
    outStr = std::wstring(inStr.begin(), inStr.end());
  }

  /////////////////////////////////////////////////////////////////////////
  void getTimeWithFormat(const std::string& format, std::string& time)
  {
    Time t = now();
    auto localtime = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&localtime, format.c_str());
    time = oss.str();
  }

  /////////////////////////////////////////////////////////////////////////
  Time now()
  {
    return std::time(nullptr);
  }
}
