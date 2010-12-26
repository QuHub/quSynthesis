#pragma once
namespace QuLogic {

class CSynthesizer : public CThread
{
public:
  CSynthesizer(void);
  ~CSynthesizer(void);
  DWORD Run( LPVOID /* arg */ );
  HANDLE m_hMutex;
  void Lock() { WaitForSingleObject(m_hMutex, INFINITE);}
  void Release(){::ReleaseMutex(m_hMutex);}
  void Resize(PULONGLONG &pBuf);
  void Process(ULONGLONG inTerm, ULONGLONG outTerm);
  ULONGLONG Propogate(ULONGLONG outTerm);
  ULONGLONG QuantumCost();
  ULONGLONG GateCost(int i);
  ULONGLONG ControlLines(ULONGLONG n);


public:
  PULONGLONG m_pTarget;   // Array holding target bit value
	PULONGLONG m_pControl;  // Array holding control line values
	ULONGLONG m_nGates;     // Number of gates found
	ULONGLONG m_nBits;      // Number of bits under synthesis (MIGHT DEPRECATE)
  ULONGLONG m_nBufSize;   // Size of allocation buffer
  int m_nOnes[256];       // Number of ones for each value of a byte....
};

}