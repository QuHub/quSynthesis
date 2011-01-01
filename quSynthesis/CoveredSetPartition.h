#pragma once
#include "QuAlgorithm.h"

// any nBits are divided into S|R, where S is the number of covering bits, and R is the remaining Hasse Bits
#define M 2

namespace QuLogic {
  class CoveredSetPartition :   public QuAlgorithm
  {
  public:
    int m_nSets;
    vector<CHasse*> m_pInput;
    ULONGLONG m_nQuantumCost;

    CoveredSetPartition(void)  {}
    CoveredSetPartition(int nBits) : QuAlgorithm(nBits)
    {
      m_nSets = (int)Math::Pow(2,M);
      for (int i=0; i<m_nSets; i++)
        m_pInput.push_back(new CHasse(nBits - M, i));
    }

    void virtual Synthesize(PULONGLONG pOut) 
    {
      m_pOut = pOut;
      Input();
      gQueue.Push(this);
    }

    PULONGLONG Input()
    {
      PULONGLONG p = m_pIn = new ULONGLONG[m_nTerms];
      for (int i=0; i<m_nSets; i++) {
        CopyMemory(p, m_pInput[i]->m_pSequence, sizeof(ULONGLONG) * m_pInput[i]->m_nTerms);
        p += m_pInput[i]->m_nTerms;
      }
      return m_pIn;
    }

    void Mutate()
    {

    }

    ~CoveredSetPartition(void) 
    {
      for (int i=0; i<m_pInput.size(); i++)
        delete m_pInput[i];
    }
  };
}
