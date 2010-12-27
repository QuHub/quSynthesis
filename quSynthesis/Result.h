#pragma once

#define RECYCLE(x) if (x) {delete x; x=NULL;}
#define COPY(dst, src, n) {if(dst) delete dst; dst = new ULONGLONG[n]; CopyMemory(dst, src, n);}
namespace QuLogic {
  class CResult
  {
  public:
    HANDLE m_hMutex;
    ULONGLONG m_nCost;
    PULONGLONG m_pIn;
    PULONGLONG m_pOut;
    PULONGLONG m_pTarget;
    PULONGLONG m_pControl;

    void Reset()
    {
      Lock();
      m_nCost = MAXLONGLONG;

      RECYCLE(m_pTarget);
      RECYCLE(m_pControl);
      RECYCLE(m_pIn);
      RECYCLE(m_pOut);
      Release();
    }

    CResult(void)
    {
      CResult::m_hMutex = CreateMutex(NULL, false, NULL);
    }

    void Save(PULONGLONG pTarget, PULONGLONG pControl, ULONGLONG nBufSize, ULONGLONG nQuantumCost, CQueueItem *qi)
    {
      Lock();
      if (m_nCost > nQuantumCost) {
        COPY(m_pTarget, pTarget, nBufSize);
        COPY(m_pControl, pControl, nBufSize);
        COPY(m_pIn, qi->pIn, qi->nSize);
        COPY(m_pOut, qi->pOut, qi->nSize);
        m_nCost = nQuantumCost;
      }
      Release();
    }

    void PrintResult()
    {
      cout << "QuantumCost: " << m_nCost << "\n";

    }

    void Lock() { WaitForSingleObject(m_hMutex, INFINITE); }
    void Release(){::ReleaseMutex(m_hMutex); }
  };
}
