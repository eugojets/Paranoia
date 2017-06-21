#pragma once
#include "stdafx.h"
#include <deque>

template <typename T>
class BoundedDeque
{
private:
  std::deque<T> Deque;
  unsigned int MaxSize;

public:
  BoundedDeque<T>(unsigned int max = 5);
  virtual ~BoundedDeque<T>();
  void Push(T data);
  void Pop();
  void Clear();
  const T Front();
  const T Back();
  const unsigned int GetSize();
  const unsigned int GetMaxSize();
  const bool Empty();
};

/////////////////////////////////////////////////////////////////////////
template<typename T>
inline BoundedDeque<T>::BoundedDeque(unsigned int max = 5) : MaxSize(max)
{
}

/////////////////////////////////////////////////////////////////////////
template<typename T>
inline BoundedDeque<T>::~BoundedDeque()
{
  Clear();
}

/////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BoundedDeque<T>::Push(T data)
{
  if(Deque.size() == MaxSize)
  {
    Deque.pop_front();
  }
  Deque.push_back(data);
}

/////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BoundedDeque<T>::Pop()
{
  if(!Deque.empty())
  {
    Deque.pop_front();
  }
}

/////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BoundedDeque<T>::Clear()
{
  std::deque<T>().swap(Deque);
}

/////////////////////////////////////////////////////////////////////////
template<typename T>
inline const T BoundedDeque<T>::Front()
{
  return Deque.empty() ? T() : Deque.front();
}

/////////////////////////////////////////////////////////////////////////
template<typename T>
inline const T BoundedDeque<T>::Back()
{
  return Deque.empty() ? T() : Deque.back();
}

/////////////////////////////////////////////////////////////////////////
template<typename T>
inline const unsigned int BoundedDeque<T>::GetSize()
{
  return Deque.size();
}

/////////////////////////////////////////////////////////////////////////
template<typename T>
inline const unsigned int BoundedDeque<T>::GetMaxSize()
{
  return MaxSize;
}

/////////////////////////////////////////////////////////////////////////
template<typename T>
inline const bool BoundedDeque<T>::Empty()
{
  return Deque.empty();
}
