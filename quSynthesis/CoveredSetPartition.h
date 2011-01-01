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

    CoveredSetPartition(void)  {}
    CoveredSetPartition(int nBits) : QuAlgorithm(nBits)
    {
      m_nSets = (int)Math::Pow(2,M);
      for (int i=0; i<m_nSets; i++)
        m_pInput.push_back(new CHasse(nBits - M));
    }

    ~CoveredSetPartition(void) 
    {
      for (int i=0; i<m_pInput.size(); i++)
        delete m_pInput[i];
    }
  };
}
