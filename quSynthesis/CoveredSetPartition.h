#pragma once
#include "QuAlgorithm.h"

// any nBits are divided into S|R, where S is the number of covering bits, and R is the remaining Hasse Bits
#define M QuLogic::PartitionSize

namespace QuLogic {
  extern int *BandBoundary;
  extern int PartitionSize;
  class CoveredSetPartition :   public QuAlgorithm
  {
  public:
    int m_nSets;
    vector<CHasse*> m_pInput;
    ULONGLONG m_nQuantumCost;
    int NumBands() {return m_nSets * (m_nBits-M+1);}
   __declspec(property(get = NumBands)) int nBands;

    CoveredSetPartition(void)  {}
    CoveredSetPartition *Copy() {return new CoveredSetPartition(*this);}

    // Copy Constructor;
    CoveredSetPartition(const CoveredSetPartition& base) : QuAlgorithm(base)
    {
      // Notice that we're not copying the Hasse Structures, just the m_pIn array of inputs...
      m_pInput.clear();
      m_pIn = new ULONGLONG[m_nTerms];
      m_nSets = base.m_nSets;
      CopyMemory(m_pIn, base.m_pIn, m_nTerms*sizeof(ULONGLONG));
    }

    CoveredSetPartition(int nBits) : QuAlgorithm(nBits)
    {
      m_nSets = (int)Math::Pow(2,M);

      for (int i=0; i<m_nSets; i++)
        m_pInput.push_back(new CHasse(nBits - M, i));

      PULONGLONG p = m_pIn = new ULONGLONG[m_nTerms];
      for (int i=0; i<m_nSets; i++) {
        p += m_pInput[i]->GetSequence(p, i<<(m_nBits-M));
      }

      // Allocate array of band boundaries for crossover operations.
      if(BandBoundary) return;
      BandBoundary = new int[nBands];
      
      for (int i=0, n=0, k=0; i<m_nSets; i++) {
        int bands = m_pInput[i]->m_nBands;
        for (int j=0; j<m_pInput[i]->m_nBands; j++) {
          n += CGlobals::nCr(m_nBits-M, j);
          BandBoundary[k++] = n;
        }
      }
    }

    QuAlgorithm *SinglePointCrossOver(QuAlgorithm *other, double Prob)
    {
      QuAlgorithm *p = m_QuantumCost < other->m_QuantumCost ? Copy() : other->Copy();     // Best Fit

      if (Rand::NextDouble() < Prob) {
        QuAlgorithm *q = m_QuantumCost < other->m_QuantumCost ? other : this;             // Less Fit

        int nFirst = QuLogic::BandBoundary[Rand::NextInteger(nBands)];

        CopyMemory(p->m_pIn + nFirst, q->m_pIn + nFirst, (m_nTerms - nFirst) * sizeof(ULONGLONG));
      }
      return p;
    }

    void Synthesize(PULONGLONG pOut) 
    {
      m_pOut = pOut;
      gQueue.Push(this);
    }

    void Mutate(double Prob)
    {
      if (Rand::NextDouble() > Prob) return;

      int band = Rand::NextInteger(nBands-1);
      int nStart= QuLogic::BandBoundary[band];
      int nEnd = QuLogic::BandBoundary[band+1];
      int nFirst = nStart + Rand::NextInteger(nEnd - nStart);
      int nSecond = nStart + Rand::NextInteger(nEnd - nStart);
      ULONGLONG tmp = m_pIn[nFirst];
      m_pIn[nFirst] = m_pIn[nSecond];
      m_pIn[nSecond] = tmp;
      //Console::WriteLine("Mutate: {0}<=>{1}", nFirst, nSecond);
    }

    ~CoveredSetPartition(void) 
    {
      delete m_pIn;
      for (int i=0; i<m_pInput.size(); i++)
        delete m_pInput[i];
    }
  };
}
