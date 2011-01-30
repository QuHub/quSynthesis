#pragma once

using namespace System;
using namespace System::IO;
#define RECYCLE(x) if (x) {delete x; x=NULL;}
#define COPY(dst, src, n) {if(dst) delete dst; dst = new ULONGLONG[n]; CopyMemory(dst, src, n);}
namespace QuLogic {
  class CResult
  {
  public:
    QuAlgorithm *my_pAlgo;

    CResult(void) { my_pAlgo = NULL; }

    void SaveResult(QuAlgorithm *algo)
    {
      if(my_pAlgo) delete my_pAlgo;

      my_pAlgo = algo->Clone();
    }

    void PrintResult()
    {
      //Directory::CreateDirectory( String::Format("..\\..\\SaveData\\{0}-bits", algo->m_nBits));
      Console::WriteLine("BestCost: {0}", my_pAlgo->m_QuantumCost); 
    }

  };
}

