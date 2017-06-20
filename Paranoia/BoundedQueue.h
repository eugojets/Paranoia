#pragma once
#include "stdafx.h"
#include <queue>

template <typename T>
class BoundedQueue
{
private:
  std::queue<T> Queue;
  unsigned int MaxSize;

public:
  BoundedQueue<T>(unsigned int max = 5);
  virtual ~BoundedQueue<T>();
  void Push(T data);
  void Pop();
  void Clear();
  const T Front();
  const unsigned int GetSize();
  const unsigned int GetMaxSize();
  const bool Empty();
};

/////////////////////////////////////////////////////////////////////////
template<typename T>
inline BoundedQueue<T>::BoundedQueue(unsigned int max = 5) : MaxSize(max)
{
}

/////////////////////////////////////////////////////////////////////////
template<typename T>
inline BoundedQueue<T>::~BoundedQueue()
{
  Clear();
}

/////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BoundedQueue<T>::Push(T data)
{
  if(Queue.size() == MaxSize)
  {
    Queue.pop();
  }
  Queue.push(data);
}

/////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BoundedQueue<T>::Pop()
{
  if(!Queue.empty())
  {
    Queue.pop();
  }
}

/////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BoundedQueue<T>::Clear()
{
  std::queue<T>().swap(Queue);
}

/////////////////////////////////////////////////////////////////////////
template<typename T>
inline const T BoundedQueue<T>::Front()
{
  return Queue.empty() ? T() : Queue.front();
}

/////////////////////////////////////////////////////////////////////////
template<typename T>
inline const unsigned int BoundedQueue<T>::GetSize()
{
  return Queue.size();
}

/////////////////////////////////////////////////////////////////////////
template<typename T>
inline const unsigned int BoundedQueue<T>::GetMaxSize()
{
  return MaxSize;
}

/////////////////////////////////////////////////////////////////////////
template<typename T>
inline const bool BoundedQueue<T>::Empty()
{
  return Queue.empty();
}
