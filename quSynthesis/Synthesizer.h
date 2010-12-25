#pragma once

class CSynthesizer : public CThread
{
public:
  CSynthesizer(void);
  ~CSynthesizer(void);
  DWORD Run( LPVOID /* arg */ );
  HANDLE m_hMutex;
  void Lock() { WaitForSingleObject(m_hMutex, INFINITE);}
  void Release(){::ReleaseMutex(m_hMutex);}
};

