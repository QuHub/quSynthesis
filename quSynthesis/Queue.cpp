#include "StdAfx.h"
#include "Queue.h"

using namespace System;

QuLogic::CQueue gQueue;
namespace QuLogic {

  CQueue::CQueue(void) : m_hMutex(NULL)
  {
    m_hMutex = ::CreateMutexA(NULL, false, NULL);
  }


  CQueue::~CQueue(void)
  {
    Release();
  }

  void CQueue::Push(PULONGLONG pIn, PULONGLONG pOut, ULONGLONG nBits)
  {
    Lock(); 
    CQueueItem *qi = new CQueueItem();
    qi->pOut = pOut;
    qi->pIn =  pIn;
    qi->nSize = (ULONGLONG)Math::Pow(2,(double)nBits);
    qi->nBits = nBits;

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

}