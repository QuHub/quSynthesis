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

void CQueue::Push(PULONGLONG pIn, PULONGLONG pOut, ULONGLONG nSize)
{
  Lock(); 
  CQueueItem *qi = new CQueueItem();
  qi->pOut = pOut;
  qi->pIn =  pIn;
  qi->nSize = nSize;

  m_Queue.push(qi);
  Release();
}


CQueueItem *CQueue::Pop()
{
  if (Empty())
    return NULL;

  Lock(); 
  CQueueItem *qi  = m_Queue.front();
  m_Queue.pop();
  Release();
  return qi;
}

