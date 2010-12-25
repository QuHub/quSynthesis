#pragma once

class CQueueItem
{
public:
  ULONGLONG *pIn;
  ULONGLONG *pOut;
  ULONGLONG nSize;
};

class CQueue
{
public:
  CQueue(void);
  ~CQueue(void);
  void Push(PULONGLONG pIn, PULONGLONG pOut, ULONGLONG nSize);
  CQueueItem *Pop();
  bool Empty(){return m_Queue.size() <= 0;}

private:
  HANDLE m_hMutex;
  queue<CQueueItem*> m_Queue;
  void Lock() { WaitForSingleObject(m_hMutex, INFINITE);}
  void Release(){::ReleaseMutex(m_hMutex);}
};

