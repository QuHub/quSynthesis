#pragma once
#define CAPACITY  1024*100
#define THRESHOLD 1024
#define DELTA 1024*50

class QuSynthesizer
{
public:
  PINT m_pTarget;   // Array holding target bit value
	PINT m_pControl;  // Array holding control line values
  PINT m_pOperation;  // Array holding control line values
	int m_nGates;     // Number of gates found
	int m_nBits;      // Number of bits under synthesis (MIGHT DEPRECATE)
  int m_nBufSize;   // Size of allocation buffer
  int m_nOnes[256];       // Number of ones for each value of a byte....

  QuSynthesizer(void)
  {
  }
  
  void Allocate(int nBits)
  {
 		m_nBufSize = CAPACITY;
		m_pTarget = new int[m_nBufSize];
		m_pControl = new int[m_nBufSize];
		m_pOperation = new int[m_nBufSize];

		m_nGates = 0;
    m_nBits = nBits;
  }
  
  void virtual Process(int inTerm, int outTerm)
  {
      throw("Must implement");
  }

  int virtual QuantumCost()
  {
    throw("Must implement");
  }
};

