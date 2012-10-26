#pragma once
#include "stdafx.h"

namespace QuLogic {
  public ref class Function {
    String m_Name;
    int m_nBits;
    int m_nTerms;
    PULONGLONG m_pIn;
    PULONGLONG m_pOut;
 
  public:

    Function(String Name, int nBits, int nTerms, PULONGLONG pIn, PULONGLONG pOut) {
      m_Name = Name;
      m_nTerms = nTerms;
      m_nBits = nBits;
      m_pIn = pIn;
      m_pOut = pOut;
    }

    ~Function()
    {
      delete m_pIn;
      delete m_pOut;
    }
  };
}
