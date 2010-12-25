#pragma once
#include "QuAlgorithm.h"

namespace QuLogic {
ref class CMiller : QuAlgorithm
{
public:
  PULONGLONG m_pIn;
public:

  CMiller(int nBits) : QuAlgorithm(nBits)
  {
    Name = "Miller";

    // Allocate input vector and fill it with natural binary order (Miller style).
    m_pIn = new ULONGLONG[m_nTerms];
    PULONGLONG p = m_pIn;
    for (int i=0; i < m_nTerms; i++)
      *p++ = i;
  }

  void virtual Synthesize(PULONGLONG pOut) override
  {
    cout << "Ready to synthesize function\n";
    gQueue.Push(m_pIn, pOut, m_nTerms);
    cout << "Pushed it to Queue\n";

    WaitForQueue();
    cout << "Done after a long wait\n";
  }
};

}