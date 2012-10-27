#pragma once
#include "stdafx.h"

namespace QuLogic {
  public ref class Function {
    String^ m_Name;
    int m_nBits;
    int m_nTerms;
 
  public:
    PULONGLONG m_pIn;
    PULONGLONG m_pOut;
    String^ Name() {return m_Name;}
    int nBits() {return m_nBits;}
    int nTerms() {return m_nTerms;}

    Function(String^ Name, int nBits, int nTerms, PULONGLONG pIn, PULONGLONG pOut) {
      m_Name = Name;
      m_nTerms = nTerms;
      m_nBits = nBits;
      
      m_pIn = new ULONGLONG[nTerms]; 
      m_pOut = new ULONGLONG[nTerms];
      CopyMemory(m_pIn, pIn, nTerms * sizeof(ULONGLONG));
      CopyMemory(m_pOut, pOut, nTerms * sizeof(ULONGLONG));
    }

    ~Function()
    {
      delete m_pIn;
      delete m_pOut;
    }
  };
}
