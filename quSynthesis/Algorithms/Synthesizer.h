#pragma once
namespace QuLogic {

class CSynthesizer : public CThread, CGlobals
{
public:
  CSynthesizer(void);
  ~CSynthesizer(void);
  DWORD Run( LPVOID /* arg */ );
  void Lock() { WaitForSingleObject(m_hMutex, INFINITE); Print("Acquired Lock");}
  void Release(){::ReleaseMutex(m_hMutex); Print("Released Lock");}
  void Resize();
  void Process(ULONGLONG inTerm, ULONGLONG outTerm);
  ULONGLONG Propogate(ULONGLONG outTerm);
  ULONGLONG QuantumCost();
  inline ULONGLONG GateCost(int i);
  ULONGLONG ControlLines(ULONGLONG n);


public:
  HANDLE m_hMutex;
  PULONGLONG m_pTarget;   // Array holding target bit value
	PULONGLONG m_pControl;  // Array holding control line values
	ULONGLONG m_nGates;     // Number of gates found
	ULONGLONG m_nBits;      // Number of bits under synthesis (MIGHT DEPRECATE)
  ULONGLONG m_nBufSize;   // Size of allocation buffer
  int m_nOnes[256];       // Number of ones for each value of a byte....
};

}