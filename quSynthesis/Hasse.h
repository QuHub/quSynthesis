#pragma warning(disable:4996)
#pragma once

namespace QuLogic {
  extern int Radix;
  extern int RadixBits;
  extern int RadixMask; 
  class CHasse 
  {
  public:
    static bool m_fInitialized;
    static int m_nBits;
    static int m_nTerms;
    static int m_nBands;
    static vector<ULONGLONG> *m_pBands;

    static int nBands(int nBits)
    {
      // A Radix (x) Hasse diagram for n variables we will have k Levels (bands), where the
      // sum of digits for each band increases by one from band to band, with 
      // the maximum sum at the top consists of (Radix - 1) repeated n times.  So:
      //   Number of bands = (Radix-1)*n + 1 (the 1 is for 0000...00)
      //   Example: For 2 bits:  
      //             Binary                                 Ternary
      //       Band                Digit Sum           Band                Digit Sum              
      //       {000},                 0                {00},                 0     
      //       {010, 100, 001},       1                {01, 10},             1     
      //       {011, 110, 101},       2                {02, 11, 20},         2     
      //       {111},                 3                {12, 21},             3     
      //                                               {22}                  4     
      //        Total: 1*3 + 1 = 4 bands.              Total: 2*2 + 1 = 5 bands
      //
      return (QuLogic::Radix - 1) * nBits + 1;  // For  nBits + 1
    }

    static void Initialize(int nBits)
    {
      // Initialize it only once.
      if(m_fInitialized) return;

      // Allocate space for bands
      m_nBits = nBits;
      m_nTerms = (int)Math::Pow(QuLogic::Radix, nBits);

      m_nBands = nBands(nBits);
      m_pBands = new vector<ULONGLONG>[m_nBands];   // Delete only once at the end of the or on TotalReset();

      // Insert each number into its band based on the sum of its digits
      for (int i=0; i<m_nTerms; i++) {
        int termInRadixDigits = CGlobals::RadixDigits(i);
        m_pBands[CGlobals::BandSum(termInRadixDigits)].push_back(termInRadixDigits);
      }

      m_fInitialized = true;
    }

  public:
    CHasse(int nBits, int offset)
    {
      Initialize(nBits);

      for (int i=0; i<m_nBands; i++) {
        random_shuffle(m_pBands[i].begin(), m_pBands[i].end());
      }
    }

    // Serialize Hasse sequence and copy it into the buffer p
    // For Set Partitions, add the offset to create the top bits of the set
    //
    int GetSequence(PULONGLONG p, int offset)
    {
      int n=0;
      for (int i=0; i<m_nBands; i++) {
        for (int j=0; j<m_pBands[i].size(); j++) {
          *p++ = m_pBands[i][j] + offset;
          n++;
        }
      }
      return n;
    }

    ~CHasse()
    {
    }
  };

}