#pragma once

ref class Utility
{
public:
  static int m_nBits=0;
  Utility(void)
  {
  }

  static vector<int> *TernarySequence(int nBits)
  {
    int nTerms = (int) pow(3.0, nBits);
    vector<int> *pv = new vector<int>(nTerms);

    for (int i=0; i<nTerms; i++) 
      pv->at(i) = ToBinaryTrit(i);

    return pv;
  }

  static int NonZeros(int term)
  {
    int count = 0;
    int mask = 3;

    for (int i=0; i<m_nBits; i++) {
      if( (term & mask) != 0)
        count++;
       mask <<= 2;
    }

    return count;
  }

  static int ToBinaryTrit(int term)
  {
    int t=0;
    
    for (int i=0; i<m_nBits; i++) {
      t += (term % 3) << (2*i);
      term /= 3;
    }
    
    return t;
  }

  static int ToDecimal(int term)
  {
    int t=0;
    
    for (int i=0; i<m_nBits; i++) {
      t += (term & 3) * (int)pow(3.0, i);
      term >>= 2;
    }
    
    return t;
  }


  static string ToString(int term)
  {
    string to_s[] = {"0", "1", "2"};
    string sTmp;
    for (int i=0; i<m_nBits; i++) {
      sTmp = to_s[(term & 3)] + sTmp;
      term >>= 2;
    }
    return sTmp;
  }
};

