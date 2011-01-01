#include "StdAfx.h"
#include "Queue.h"

using namespace System;

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
    CQueueItem *qi = new CQueueItem();
    qi->pOut = pOut;
    qi->pIn =  pIn;
    qi->nSize = (ULONGLONG)Math::Pow(2,(double)nBits);
    qi->nBits = nBits;

    Print("In: Push");
    Lock(); 
    m_Queue.push(qi);
    Release();
    Print("Out: Push");
  }


  CQueueItem *CQueue::Pop()
  {
    Print("In: Pop");
    Lock();
    if (Empty()) {
      Print("Out: Pop Empty");
      Release();
      return NULL;
    }
    CQueueItem *qi = m_Queue.front();
    m_Queue.pop();
    Release();
    Print("Out: Pop");
    return qi;
  }

}