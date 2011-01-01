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

  void CQueue::Push(QuAlgorithm *qa)
  {
    Print("In: Push");
    Lock(); 
    m_qaQueue.push(qa);
    Release();
    Print("Out: Push");
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


  QuAlgorithm *CQueue::Pop()
  {
    Print("In: Pop");
    Lock();
    if (Empty()) {
      Print("Out: Pop Empty");
      Release();
      return NULL;
    }
    QuAlgorithm *qa = m_qaQueue.front();
    m_qaQueue.pop();
    Release();
    Print("Out: Pop");
    return qa;
  }

}