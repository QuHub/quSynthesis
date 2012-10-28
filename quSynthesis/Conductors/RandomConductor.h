#pragma once
#include "Function.h"

#define N_BATCH 1000
#define N_RUNS  100

namespace QuLogic 
{
  class RandomConductor: public QuConductor {
    QuAlgorithm *m_pAlgo[N_BATCH];

  public:
    ~RandomConductor()
    {
    }

    RandomConductor(Function^ function) : QuConductor(function)
    {
    }

    void Synthesize()
    {
      ULONGLONG bestCost=MAXINT64;

      for (int j=0; j<N_RUNS; j++) {
        Console::WriteLine("Run: {0}", j);
        for (int i=0; i<N_BATCH; i++) { 
          m_pAlgo[i] = new CoveredSetPartition(m_function);
          m_pAlgo[i]->Synthesize();
        }
        WaitForQueue();

        for (int i=0; i<N_BATCH; i++) { 
          ULONGLONG qCost = m_pAlgo[i]->m_QuantumCost;
          if (bestCost > qCost) {
            bestCost = qCost;
            SaveResult(m_pAlgo[i]);
          }
          delete m_pAlgo[i];
        }
      }
		  PrintResult(0);
    }
  };
}

