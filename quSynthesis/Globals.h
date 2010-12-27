#pragma once

namespace QuLogic {
  class CGlobals 
  {
  public:
    int m_Ones[256];
    HANDLE m_hPrintMutex;
    inline int Ones(int i){return m_Ones[i];}
    CGlobals(void)
    {
      for (int i=0; i<256; i++)
        m_Ones[i] = NumOfOnes(i);
    
      if (!ghPrintMutex)
        ghPrintMutex = CreateMutex(NULL, false, NULL);
    }


    int CGlobals::NumOfOnes(int num)
    {
      int nCount = 0;
      for (int i=0; i<8; i++) {
        nCount += num & 1;
        num >>= 1;
      }
      return nCount;
    }
#ifndef Print
    void CGlobals::Print(char *p)
    {
      WaitForSingleObject(ghPrintMutex, INFINITE); 
      cout << GetCurrentThreadId() << ": " << p << "\n";
      ::ReleaseMutex(ghPrintMutex); 
    }
#endif
    void Lock(HANDLE hMutex) { WaitForSingleObject(hMutex, INFINITE); }
    void Release(HANDLE hMutex){::ReleaseMutex(hMutex); }
  };
}
