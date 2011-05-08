#pragma once
class QuSynthesizer
{
public:
  PULONGLONG m_pTarget;   // Array holding target bit value
	PULONGLONG m_pControl;  // Array holding control line values
  PULONGLONG m_pOperation;  // Array holding control line values
	ULONGLONG m_nGates;     // Number of gates found
	ULONGLONG m_nBits;      // Number of bits under synthesis (MIGHT DEPRECATE)
  ULONGLONG m_nBufSize;   // Size of allocation buffer
  int m_nOnes[256];       // Number of ones for each value of a byte....

  QuSynthesizer(void)
  {
  }
  
  void Before(int nBits)
  {
 		m_nBufSize = 10*1024;
		m_pTarget = new ULONGLONG[m_nBufSize];
		m_pControl = new ULONGLONG[m_nBufSize];
		m_pOperation = new ULONGLONG[m_nBufSize];

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

