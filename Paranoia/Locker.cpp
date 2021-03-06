#include "stdafx.h"
#include "Locker.h"

/////////////////////////////////////////////////////////////////////////
Locker::Locker(std::mutex &m) : mutex(m)
{
  m.lock();
}

/////////////////////////////////////////////////////////////////////////
Locker::~Locker()
{
  mutex.unlock();
}
