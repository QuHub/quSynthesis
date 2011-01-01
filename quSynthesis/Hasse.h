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
      m_pBands = new vector<ULONGLONG>[m_nBands];

      // Allocate space for each bands
      for (int i=0; i<m_nTerms; i++)
        m_pBands[CGlobals::Ones(i)].push_back(i);

      m_fInitialized = true;
    }

  public:
    PULONGLONG m_pSequence;
    CHasse(int nBits, int offset)
    {
      Initialize(nBits);

      m_pSequence = new ULONGLONG[m_nTerms];
      PULONGLONG p=m_pSequence;
      for (int i=0; i<m_nBands; i++) {
        random_shuffle(m_pBands[i].begin(), m_pBands[i].end());
        CopyMemory(p, &m_pBands[i][0], m_pBands[i].size() * sizeof(ULONGLONG));
        p += m_pBands[i].size();
      }

      // Add offset to values
      for (int i=0; i<m_nTerms; i++)
        m_pSequence[i] += offset << nBits;
    }

    ~CHasse()
    {
      delete m_pSequence;
    }
  };

}