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
    string m_GAParams;

    void SaveResult(QuAlgorithm *algo)
    {
      if(my_pAlgo) delete my_pAlgo;

      my_pAlgo = algo->Clone();


	  
    }

    void PrintResult(int iteration)
    {
		Directory::CreateDirectory( String::Format("..\\SaveData\\{0}-bits", my_pAlgo->m_nBits));
		StreamWriter ^file = gcnew StreamWriter(String::Format("..\\SaveData\\{0}-bits\\{1}-iteration", my_pAlgo->m_nBits, iteration) + ".qsy");

    String^ s = gcnew String(m_GAParams.c_str());
    file->WriteLine("Configuration:  {0}", s);
	  file->WriteLine("QuantomCost:");
	  file->WriteLine(System::Convert::ToString(my_pAlgo->m_QuantumCost));
	  file->WriteLine("Input Data:");

	  for(int i=0;i<my_pAlgo->m_nTerms; i++)
		  file->Write(my_pAlgo->m_pIn[i]+",");
	  file->WriteLine();
	  file->WriteLine("Output Data:");

	  for(int i=0;i<my_pAlgo->m_nTerms; i++)
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
    }

  };
}

