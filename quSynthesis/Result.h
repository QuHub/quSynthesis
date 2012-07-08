#pragma once
#pragma managed
#include <msclr\marshal.h>
#include "StringConvertor.h"

using namespace System;
using namespace System::IO;
using namespace msclr::interop;
using namespace StringUtilities;


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

    void PrintResult(int iteration, double Time, String^ gaParams)
    {
      Directory::CreateDirectory( String::Format("..\\SaveData\\{0}-bits\\{1}", my_pAlgo->m_nBits, CGlobals::date));
	    StreamWriter ^file = gcnew StreamWriter(String::Format("..\\SaveData\\{0}-bits\\{1}\\{2}-iteration.qsy", my_pAlgo->m_nBits, CGlobals::date, iteration) );

      file->WriteLine("Genetic Algorithm parameters: {0}", gaParams);
      file->WriteLine("Time to Synthesize: {0}", Time);
      file->WriteLine("Radix: {0}", Config::Radix);
      file->WriteLine("Bit Count: {0}", my_pAlgo->m_nBits);
      file->WriteLine("Quantum Cost: {0}", my_pAlgo->m_QuantumCost);
      
      file->WriteLine("Input Sequence (decimal and in 2 bits per digit):");
      for(int i=0;i<Math::Pow(Config::Radix,(double)my_pAlgo->m_nBits); i++)
        file->Write(my_pAlgo->m_pIn[i] + ",");
      file->WriteLine();

      for(int i=0;i<Math::Pow(Config::Radix,(double)my_pAlgo->m_nBits); i++) {
        int term = my_pAlgo->m_pIn[i];
        file->Write( "{0}, ", term);
      }
      file->WriteLine();

      file->WriteLine("Output Sequence (decimal and in 2 bits per digit):");
      for(int i=0;i<Math::Pow(Config::Radix,(double)my_pAlgo->m_nBits); i++)
        file->Write(my_pAlgo->m_pOut[i]+",");
      file->WriteLine();

      for(int i=0;i<Math::Pow(Config::Radix,(double)my_pAlgo->m_nBits); i++) {
        int term = my_pAlgo->m_pOut[i];
        file->Write( "{0}, ", term);
      }
      file->WriteLine();

      array<String^>^ gates = {"+1", "+2", "01", "02", "12"};
      file->WriteLine("Control | Target | Operation");
      for(int i=0;i<my_pAlgo->m_nGates;i++) {
        StringConvertor sc(QuLogic::CGlobals::ToString(my_pAlgo->m_pControl[i]));
        file->Write( sc + " | ");
        file->Write(my_pAlgo->m_pTarget[i] + " | ");
        file->WriteLine("{0} | ", gates[my_pAlgo->m_pOperation[i]]);
      }

      file->Close();
     }
  };
}

