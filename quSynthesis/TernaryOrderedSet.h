#pragma once
#include "QuAlgorithm.h"

// any nBits are divided into S|R, where S is the number of covering bits, and R is the remaining Hasse Bits
#define M QuLogic::PartitionSize

namespace QuLogic {

  class TernaryOrderedSet :   public QuAlgorithm
  {
  public:
    int m_nBands;
    CHasse *m_pInput;

    TernaryOrderedSet(void)  {}
    TernaryOrderedSet *Copy() {return new TernaryOrderedSet(*this);}

    // Copy Constructor;
    TernaryOrderedSet(const TernaryOrderedSet& base) : QuAlgorithm(base)
    {
      m_nBands = base.m_nBands;
    }

    // ************** Constructor *******************
    TernaryOrderedSet(int nBits) : QuAlgorithm(nBits)
    {
      m_nBands = CHasse::nBands(nBits);
      m_pInput = new CHasse(nBits, 0);
      m_pIn = new ULONGLONG[m_nTerms];
      m_pInput->GetSequence(m_pIn, 0);

      // Allocate array of band boundaries for crossover operations.
      if(BandBoundary) return;
      BandBoundary = new int[m_nBands];

      for (int j=0; j < m_pInput->m_nBands; j++) {
        BandBoundary[j] = m_pInput->m_pBands[j].size();
      }
    }

    ~TernaryOrderedSet(void) 
    {
      // Delete Hasse Classes;
      delete m_pInput;
    }

    void Synthesize(PULONGLONG pOut) 
    {
      m_pOut = pOut;
      gQueue.Push(this);
    }


    //******************* GA Operators (CrossOver and Mutation) *************************
    QuAlgorithm *SinglePointCrossOver(QuAlgorithm *other, double Prob)
    {
      QuAlgorithm *p = m_QuantumCost < other->m_QuantumCost ? Copy() : other->Copy();     // Best Fit

      if (Rand::NextDouble() < Prob) {
        QuAlgorithm *q = m_QuantumCost < other->m_QuantumCost ? other : this;             // Less Fit

        int nFirst = QuLogic::BandBoundary[Rand::NextInteger(m_nBands)];

        CopyMemory(p->m_pIn + nFirst, q->m_pIn + nFirst, (m_nTerms - nFirst) * sizeof(ULONGLONG));
      }
      return p;
    }

    QuAlgorithm *TwoPointCrossOver(QuAlgorithm *other, double Prob)
    {
      QuAlgorithm *p = m_QuantumCost < other->m_QuantumCost ? Copy() : other->Copy();     // Best Fit


      if (Rand::NextDouble() < Prob) {
        QuAlgorithm *q = m_QuantumCost < other->m_QuantumCost ? other : this;             // Less Fit

        int nFirst = QuLogic::BandBoundary[Rand::NextInteger(m_nBands)];
        int nSecond = QuLogic::BandBoundary[Rand::NextInteger(m_nBands)];

        if(nSecond < nFirst) {
          int tmp = nSecond;
          nSecond = nFirst;
          nFirst = tmp;
        }
        CopyMemory(p->m_pIn + nFirst, q->m_pIn + nFirst, (nSecond - nFirst) * sizeof(ULONGLONG));
      }
      return p;
    }

    void Mutate(double Prob)
    {
      if (Rand::NextDouble() > Prob) return;

      int band = Rand::NextInteger(m_nBands-1);
      int nStart= QuLogic::BandBoundary[band];
      int nEnd = QuLogic::BandBoundary[band+1];
      int nFirst = nStart + Rand::NextInteger(nEnd - nStart);
      int nSecond = nStart + Rand::NextInteger(nEnd - nStart);

      // Mutate through swap..
      ULONGLONG tmp = m_pIn[nFirst];
      m_pIn[nFirst] = m_pIn[nSecond];
      m_pIn[nSecond] = tmp;
    }

  };
}
