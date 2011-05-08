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

    void PrintResult(int iteration)
    {
      Directory::CreateDirectory( String::Format("..\\..\\SaveData\\{0}-bits", my_pAlgo->m_nBits));
      StreamWriter ^file = gcnew StreamWriter(String::Format("..\\..\\SaveData\\{0}-bits\\{1}-iteration", my_pAlgo->m_nBits, iteration) + ".qsy");
      file->WriteLine("QuantomCost:");
      file->WriteLine(System::Convert::ToString(my_pAlgo->m_QuantumCost));
      file->WriteLine("Input Data:");
      for(int i=0;i<Math::Pow(2,(double)my_pAlgo->m_nBits); i++)
        file->Write(my_pAlgo->m_pIn[i]+",");
      file->WriteLine();
      file->WriteLine("Output Data:");
      for(int i=0;i<Math::Pow(2,(double)my_pAlgo->m_nBits); i++)
        file->Write(my_pAlgo->m_pOut[i]+",");
      file->WriteLine();

      file->WriteLine("Control:");
      for(int i=0;i<my_pAlgo->m_nGates;i++)
        file->Write(my_pAlgo->m_pControl[i]+",");
      file->WriteLine();
      file->WriteLine("Target:");
      for(int i=0;i<my_pAlgo->m_nGates;i++)
        file->Write(my_pAlgo->m_pTarget[i]+",");
      file->Close();

      /*
      cout << "gates" << my_pAlgo->m_nGates << endl;
      cout << "bits" << my_pAlgo->m_nBits << endl;
      cout << "cost" << my_pAlgo->m_QuantumCost << endl;


      Console::WriteLine("----------------------------------------------------");
      for(int i=0;i<my_pAlgo->m_nTerms;i++)
      Console::Write(my_pAlgo->m_pIn[i]+",");
      Console::WriteLine("\n----------------------------------------------------");
      for(int i=0;i<my_pAlgo->m_nTerms;i++)
      Console::Write(my_pAlgo->m_pOut[i]+",");
      Console::WriteLine("\n----------------------------------------------------");

      for(int i=0;i<my_pAlgo->m_nGates;i++)
      Console::Write(my_pAlgo->m_pControl[i]+",");
      Console::WriteLine("\n----------------------------------------------------");

      for(int i=0;i<my_pAlgo->m_nGates;i++)
      Console::Write(my_pAlgo->m_pTarget[i]+",");
      Console::WriteLine("\n----------------------------------------------------");


      Console::WriteLine("BestCost in result.h: {0}", my_pAlgo->m_QuantumCost); 
      */
    }

  };
}

