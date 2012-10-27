#pragma once
#include "Function.h"

namespace QuLogic 
{
  class PassThrough: public QuConductor {
  public:
    QuAlgorithm *m_pAlgo;

    ~PassThrough()
    {
      delete m_pAlgo;
    }

    PassThrough(Function^ function) : QuConductor(function)
    {
      m_pAlgo = new CoveredSetPartition(function);
    }

    void Synthesize()
    {
      m_pAlgo->Synthesize();
      WaitForQueue();
    }
  };
}

