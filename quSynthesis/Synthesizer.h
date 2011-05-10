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
  void Process(int inTerm, int outTerm);
  int Propogate(int outTerm);
  int QuantumCost();
  inline int GateCost(int i);
  int ControlLines(int n);


public:
  HANDLE m_hMutex;
};

}