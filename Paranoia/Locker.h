#pragma once
#include "stdafx.h"
#include <mutex>

class Locker
{
  std::mutex &mutex;
public:
  Locker(std::mutex& m);
  ~Locker();
};