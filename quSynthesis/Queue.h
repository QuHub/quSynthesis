#pragma once

namespace QuLogic {

  class CQueueItem
  {
  public:
    ULONGLONG *pIn;
    ULONGLONG *pOut;
    ULONGLONG nSize;
    ULONGLONG nBits;
  };

  class CQueue : public CGlobals
  {
  public:
    CQueue(void);
    ~CQueue(void);
    void Push(PULONGLONG pIn, PULONGLONG pOut, ULONGLONG nBits);
    CQueueItem *Pop();
    bool Empty(){return m_Queue.size() <= 0;}

  private:
    HANDLE m_hMutex;
    queue<CQueueItem*> m_Queue;
    void Lock() { WaitForSingleObject(m_hMutex, INFINITE); Print("Queue: Acquired Lock");}
    void Release(){::ReleaseMutex(m_hMutex); Print("Queue: Released Lock");}
  };
}