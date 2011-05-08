#pragma once

namespace QuLogic {
  class CGlobals 
  {
  public:
    static int m_BandSum[256];
    static bool m_fInitialized;

    HANDLE m_hPrintMutex;

    CGlobals(void)
    {
      if (!ghPrintMutex)
        ghPrintMutex = CreateMutex(NULL, false, NULL);
    }

    static int BandSum(ULONGLONG n)
    {
      int nCount=0;

      if (!m_fInitialized) {
        for (int i=0; i<256; i++)
          m_BandSum[i] = NonZeroCount(i);

        m_fInitialized = TRUE;
      }

      // Count ones for 64 bits..
      for (int i=0; i<8; i++) {
        nCount += m_BandSum[n & 0xFF];
        n >>= 8;
      }
      return nCount;
    }

  static string ToString(int term)
  {
    string to_s[] = {"0", "1", "2"};
    string sTmp;
    for (int i=0; i<QuLogic::nBits; i++) {
      sTmp = to_s[(term & 3)] + sTmp;
      term >>= 2;
    }
    return sTmp;
  }
    static int RadixDigits(int term)
    {
      int t=0;

      for (int i=0; i<QuLogic::nBits; i++) {
        t += (term % QuLogic::Radix) << (QuLogic::RadixBits*i);
        term /= 3;
      }

      return t;
    }

    static int CGlobals::NonZeroCount(int num)
    {
      int nCount = 0;
      for (int i=0; i<8; i++) {
        nCount += num & (QuLogic::RadixMask);         
        num >>= QuLogic::RadixBits;
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

    // nCr = n!/r!(n-r!)
    static ULONGLONG nCr(int n, int r)
    {
      if (n==r || r==0) return 1;

      // Use the largest one for r!
      int nMr = n -r;
      if (r < nMr) r = nMr;
      nMr = n - r;


      ULONGLONG nMrF = 1;
      do 
      nMrF *= nMr;
      while(--nMr);

      // Calc n!/r!
      ULONGLONG c = 1;
      while (r<n)
        c *= ++r;

      return c / nMrF;
    }

  };
}
