#include "StdAfx.h"
#include "Queue.h"

CQueue gQueue;

CQueue::CQueue(void) : m_hMutex(NULL)
{
  m_hMutex = ::CreateMutexA(NULL, false, NULL);
}


CQueue::~CQueue(void)
{
  Release();
}

void CQueue::Push(CQueueItem &qi)
{
  Lock(); 
  m_inQueue.push(qi.pIn);
  m_outQueue.push(qi.pOut);
  Release();
}


void CQueue::Pop(CQueueItem &qi)
{
  if (m_inQueue.size() <= 0)
    return;

  Lock(); 
  qi.pIn = m_inQueue.front();
  qi.pOut = m_outQueue.front();
  m_inQueue.pop();
  m_outQueue.pop();
  Release();
}

void CQueue::Lock()
{
  WaitForSingleObject(m_hMutex, INFINITE);
}


void CQueue::Release()
{
  ::ReleaseMutex(m_hMutex);
}