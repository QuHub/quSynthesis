#pragma once
#pragma managed

using namespace System;
using namespace System::IO;
#define RECYCLE(x) if (x) {delete x; x=NULL;}
#define COPY(dst, src, n) {if(dst) delete dst; dst = new int[n]; CopyMemory(dst, src, n);}
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

    void PrintResult(int iteration, double Time)
    {
      Directory::CreateDirectory( String::Format("..\\SaveData\\{0}-bits", my_pAlgo->m_nBits));
      StreamWriter ^file = gcnew StreamWriter(String::Format("..\\SaveData\\{0}-bits\\{1}-iteration", my_pAlgo->m_nBits, iteration) + ".qsy");
      file->WriteLine("QuantomCost: {0}  Seconds: {1}", my_pAlgo->m_QuantumCost, Time);
      file->WriteLine("Input Data:");
      for(int i=0;i<Math::Pow(Config::Radix,(double)my_pAlgo->m_nBits); i++)
        file->Write(my_pAlgo->m_pIn[i]+",");
      file->WriteLine();
      file->WriteLine("Output Data:");
      for(int i=0;i<Math::Pow(Config::Radix,(double)my_pAlgo->m_nBits); i++)
        file->Write(my_pAlgo->m_pOut[i]+",");
      file->WriteLine();

      file->WriteLine("Control | Target | Operation");
      for(int i=0;i<my_pAlgo->m_nGates;i++) {
        file->Write(my_pAlgo->m_pControl[i] +" | ");
        file->Write(my_pAlgo->m_pTarget[i] + " | ");
        file->WriteLine(my_pAlgo->m_pOperation[i] +" |");
      }

      file->Close();
     }
  };
}

