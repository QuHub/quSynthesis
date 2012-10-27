#pragma warning(disable:4996)
#pragma once

namespace QuLogic {
  class CHasse 
  {
  public:
    static bool m_fInitialized;
    static int m_nBits;
    static int m_nTerms;
    static int m_nBands;
    static vector<ULONGLONG> *m_pBands;

    static void Initialize(int nBits)
    {
      if(m_fInitialized) return;
      m_nBits = nBits;
      m_nTerms = (int)Math::Pow(2, nBits);
      m_nBands = nBits + 1;                 // For binary, nBands = nBits + 1
      m_pBands = new vector<ULONGLONG>[m_nBands];   // Delete only once at the end of the or on TotalReset();

      // Allocate space for each bands
      for (int i=0; i<m_nTerms; i++)
        m_pBands[CGlobals::Ones(i)].push_back(i);

      m_fInitialized = true;
    }

  public:
    CHasse(int nBits)
    {
      Initialize(nBits);

      for (int i=0; i<m_nBands; i++) {
        random_shuffle(m_pBands[i].begin(), m_pBands[i].end());
      }
    }

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