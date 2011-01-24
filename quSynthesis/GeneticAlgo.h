#pragma once

#define N_POP 100

namespace QuLogic 
{
  class GenticAlgo : QuAlgorithm {

  public:
    QuAlgorithm *m_algo[N_POP];

    CGeneticAlgo(int nBits, QuAlgorithm &algo)
    {
      // Setup population of individuals randomly
    }

    void virtual Synthesize(PULONGLONG pOut) 
    {

    }
  };
}

