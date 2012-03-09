#include <vector>
#include "CoreSequence.h"
#include "CudaSequence.h"

#pragma once
using namespace std;

namespace Ternary {
  const int Radix = 3;

// Ternary Gates
const int constTernaryGates[5][3]= {
  {2, 0, 1},      // 0: -1
  {1, 2, 0},      // 1: -2
  {1, 0, 2},      // 2: 01
  {2, 1, 0},      // 3: 02
  {0, 2, 1}       // 4: 12
};                

// Operation to use based on [input][output] values which are an index to the gates in the m_Op array above
// Example: 
//    Input = 2, Output = 1 => Gate 4 (From constTernaryGates above would be Swap gate 12)
const int constTernaryOperator[3][3] = {
  // Output    0, 1, 2     Input 
  {4, 2, 3},               // 0
  {2, 3, 4},               // 1
  {3, 4, 2}                // 2
};

// Bitmask two bits at a time for ternary operations.
const int gBitMask[] = {3, 3<<2, 3<<4, 3<<6, 3<<8, 3<<10, 3<<12, 3<<14, 3<<16};

// Bring down the ith trit to the lower two LSB bits.
#define BIT(x,i) ((x & gBitMask[i]) >> 2*i)

  class CoreSynthesizer
  {
  public:
    int m_nBits;
    int m_nTerms;
    int m_nGates;
    int m_QuantumCost;
    PCHAR Name;
    vector<CoreSequence *> m_sequences;

    PINT m_pIn;
    PINT m_pOut;
    PINT m_pTarget;
    PINT m_pControl;
    PINT m_pOperation;
    int  m_outputBlockSize;
    CudaSequence m_seq;

//    gcroot<Type^> m_SynthesisAlgo;
    
  public:
    CoreSynthesizer(void){}

    CoreSynthesizer(int nBits)
    {
      m_nBits = nBits;
      m_nTerms = (int)pow(Radix,(double)nBits);
      m_nGates = 0;
      m_pTarget = m_pControl = m_pOperation = NULL;
      m_seq.m_outputBlockSize = 200 * 1024 * sizeof(int);
      m_seq.m_nTerms = m_nTerms;
    }


    // Copy constructor
    CoreSynthesizer(const CoreSynthesizer& base)
    {
      //m_nBits = base.m_nBits;
      //m_nTerms = base.m_nTerms;
      //m_nGates = base.m_nGates;
      //m_QuantumCost = base.m_QuantumCost;
      //Name = base.Name;
      //m_pOut = base.m_pOut;

      //m_pIn = new int[m_nTerms];
      //CopyMemory(m_pIn, base.m_pIn, m_nTerms*sizeof(int));
      //m_pTarget = m_pControl = m_pOperation = NULL;
    }

    vector<CoreSequence *> CoreSynthesizer::Results()
    {
      return m_sequences;
    }

    ~CoreSynthesizer()
    {
      for(int i=0; i<m_sequences.size(); i++)
        delete m_sequences[i];
    }
    void Dump()
    {
      //cout << "In   |    Out" << endl;
      //for (int i=0; i<m_nTerms; i++)
      //  cout << m_pIn[i] << " | " << m_pOut[i] << endl;

      //cout << "Target | Control | Operation" << endl;

      //for (int i=0; i<m_nGates; i++)
      //  cout << m_pTarget[i] << " | " << m_pControl[i] << " | " << m_pOperation[i] << endl;
    }

    //void virtual Synthesize(PINT pOut) {throw "Must implement this";}
    //void virtual Mutate(double Prob) {throw "Must implement this";}
    //CoreSynthesizer virtual *Copy() {throw "Must implement this";}
    //CoreSynthesizer virtual *Clone() {
    //  CoreSynthesizer *p = this->Copy();
    //  p->m_pControl = new int[m_nGates];
    //  CopyMemory(p->m_pControl, this->m_pControl, m_nGates*sizeof(int));

    //  p->m_pTarget = new int[m_nGates];
    //  CopyMemory(p->m_pTarget, this->m_pTarget, m_nGates*sizeof(int));

    //  p->m_pOperation= new int[m_nGates];
    //  CopyMemory(p->m_pOperation, this->m_pOperation, m_nGates*sizeof(int));

    //  return p;
    //}
    //CoreSynthesizer virtual *SinglePointCrossOver(CoreSynthesizer *p, double Prob){throw "Must implement this";}
    //CoreSynthesizer virtual *TwoPointCrossOver(CoreSynthesizer *p, double Prob){throw "Must implement this";}
    //void inline Delete(PINT p) {if(p) delete p;}
    //virtual ~CoreSynthesizer(){
    //  Delete(m_pIn); 
    //  Delete(m_pControl); 
    //  Delete(m_pTarget);
    //  Delete(m_pOperation);
    //}
  };
}
