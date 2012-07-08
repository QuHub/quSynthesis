#pragma once
#include "QuAlgorithm.h"

// any nBits are divided into S|R, where S is the number of covering bits, and R is the remaining Hasse Bits
#undef M
#define M Config::PartitionSize

namespace Ternary {
  namespace Algorithm {

    class NaturalOrder :   public  QuLogic::QuAlgorithm
    {
    public:

      NaturalOrder(void)  {}
      NaturalOrder *Copy() {return new NaturalOrder(*this);}

      // Copy Constructor;
      NaturalOrder(const NaturalOrder& base) : QuAlgorithm(base)
      {
      }

      // <summary>
      // 
      // <inputs>
      //
      // <outputs>
      NaturalOrder(int nBits) : QuAlgorithm(nBits)
      {
        m_pIn = new int[m_nTerms];
        for (int i=0; i<m_nTerms; i++) {
          m_pIn[i] = QuLogic::CGlobals::RadixDigits(i);
        }
      }

      // <summary>
      // 
      // <inputs>
      //
      // <outputs>
      ~NaturalOrder(void) 
      {
        // Delete Hasse Classes;
      }

      // <summary>
      // 
      // <inputs>
      //
      // <outputs>
      static void Prepare(PINT pOut)
      {
        int nTerms = (int)pow(Config::Radix, (double)Config::nBits);

        for(int i=0; i<nTerms; i++)
          pOut[i] = QuLogic::CGlobals::RadixDigits(pOut[i]);
      }

      // <summary>
      // 
      // <inputs>
      //
      // <outputs>
      void Synthesize(PINT pOut) 
      {
        m_pOut = pOut;

        Prepare(m_pOut);
        gQueue.Push(this);
      }
    };
  }
}