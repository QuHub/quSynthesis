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
    int m_nMaxTerms;
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
      m_nSets = base.m_nSets;
    }

    CoveredSetPartition(Function^ function) : QuAlgorithm(function)
    {
      m_nSets = (int)Math::Pow(2,M);

      // Construct full set of Hasse structure including partitions
      for (int i=0; i<m_nSets; i++)
        m_pInput.push_back(new CHasse(function->nBits() - M));

      m_nMaxTerms = (int) pow(2, (float)m_nBits);
      ULONGLONG *buf = new ULONGLONG[m_nMaxTerms];
      PULONGLONG p = buf;
      for (int i=0; i<m_nSets; i++) {
        p += m_pInput[i]->GetSequence(p, i<<(m_nBits-M));
      }

      CopyTermsInInput(buf);
      delete buf;
    }

    void CopyTermsInInput(PULONGLONG p)
    {
      PULONGLONG pIn = m_pIn = new ULONGLONG[m_nTerms];
      PULONGLONG pOut = m_pOut = new ULONGLONG[m_nTerms];

      for(int i=0; i<m_nMaxTerms; i++) {
        for(int j=0; j<m_nTerms; j++) {
          if(p[i] == m_function->m_pIn[j]) {
            *pIn++ = p[i];
            *pOut++ = m_function->m_pOut[j];
          }
        }
      }
    }

    void Synthesize() 
    {
      gQueue.Push(this);
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

    QuAlgorithm *TwoPointCrossOver(QuAlgorithm *other, double Prob)
    {
      QuAlgorithm *p = m_QuantumCost < other->m_QuantumCost ? Copy() : other->Copy();     // Best Fit


      if (Rand::NextDouble() < Prob) {
        QuAlgorithm *q = m_QuantumCost < other->m_QuantumCost ? other : this;             // Less Fit

        int nFirst = QuLogic::BandBoundary[Rand::NextInteger(nBands)];
        int nSecond = QuLogic::BandBoundary[Rand::NextInteger(nBands)];

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

      int band = Rand::NextInteger(nBands-1);
      int nStart= QuLogic::BandBoundary[band];
      int nEnd = QuLogic::BandBoundary[band+1];
      int nFirst = nStart + Rand::NextInteger(nEnd - nStart);
      int nSecond = nStart + Rand::NextInteger(nEnd - nStart);

      // Mutate through swap..
      ULONGLONG tmp = m_pIn[nFirst];
      m_pIn[nFirst] = m_pIn[nSecond];
      m_pIn[nSecond] = tmp;
    }

    ~CoveredSetPartition(void) 
    {
      // Delete Hasse Classes;
      for (int i=0; i<m_pInput.size(); i++)
        delete m_pInput[i];
    }
  };
}
