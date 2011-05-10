#pragma once
#include "QuAlgorithm.h"

namespace QuLogic {
class CMiller : QuAlgorithm
{
public:
  PINT m_pIn;
public:

  CMiller(int nBits) : QuAlgorithm(nBits)
  {
    Name = "Miller";

    // Allocate input vector and fill it with natural binary order (Miller style).
    m_pIn = new int[m_nTerms];
    PINT p = m_pIn;
    for (int i=0; i < m_nTerms; i++)
      *p++ = i;
  }

  void virtual Synthesize(PINT pOut) override
  {
    //gResult.Reset();
    //Print("Ready to synthesize function");
    //gQueue.Push(m_pIn, pOut, m_nBits);
    //Print("Pushed it to Queue");

    ////WaitForQueue();
    //gResult.PrintResult();
    //Print("Done after a long wait");
  }
};

}