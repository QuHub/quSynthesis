#pragma once
#include "QuAlgorithm.h"

namespace QuLogic {
class CMiller : QuAlgorithm
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
    for (int i=0; i<1000; i++) {
      Print("Ready to synthesize function");
      gQueue.Push(m_pIn, pOut, m_nBits);
      Print("Pushed it to Queue");
    }

    WaitForQueue();
    Print("Done after a long wait");
  }
};

}