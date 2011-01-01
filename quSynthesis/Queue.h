#pragma once

namespace QuLogic {

  class CQueue : public CGlobals
  {
  public:
    CQueue(void);
    ~CQueue(void);
    void Push(QuAlgorithm *qa);
    QuAlgorithm *Pop();
    bool Empty(){return m_qaQueue.size() <= 0;}

  private:
    HANDLE m_hMutex;
    queue<QuAlgorithm*> m_qaQueue;
    void Lock() { WaitForSingleObject(m_hMutex, INFINITE); Print("Queue: Acquired Lock");}
    void Release(){::ReleaseMutex(m_hMutex); Print("Queue: Released Lock");}
  };
}