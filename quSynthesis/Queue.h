#pragma once

class CQueueItem
{
public:
  int *pIn;
  int *pOut;
  int size;
};

class CQueue
{
public:
  CQueue(void);
  ~CQueue(void);
  void Push(CQueueItem &);
  void Pop(CQueueItem &);

private:
  HANDLE m_hMutex;
  queue<int *> m_inQueue;
  queue<int *> m_outQueue;
  void Lock();
  void Release();
};

