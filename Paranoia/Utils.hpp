#pragma once
#include "stdafx.h"
#include <iterator>
#include <string>
#include <sstream>
#include <vector>
#include <Windows.h>

namespace Utility
{
  std::vector<std::string> split(const std::string &s, char delim);
  void getContentsInDir(std::string dir, std::vector<std::string>& contents);
  void stringToWstring(const std::string& inStr, std::wstring& outStr);

  // Gets the name of a class
  template<typename Class>
  std::string Nameof(const Class& _)
  {
    std::vector<std::string> temp = split(typeid(Class).name(), ' ');
    return temp.size() > 2 ? temp[1] : temp[0];
  }

  template<typename Out>
  void split(const std::string &s, char delim, Out result)
  {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while(std::getline(ss, item, delim))
    {
      *(result++) = item;
    }
  }

}
