#pragma once

using namespace System;
using namespace System::IO;
#define RECYCLE(x) if (x) {delete x; x=NULL;}
#define COPY(dst, src, n) {if(dst) delete dst; dst = new ULONGLONG[n]; CopyMemory(dst, src, n);}
namespace QuLogic {
  class CResult
  {
  public:
    HANDLE m_hMutex;
    ULONGLONG m_nCost;
	ULONGLONG m_nBits;
	ULONGLONG m_nGates;
    PULONGLONG m_pIn;
    PULONGLONG m_pOut;
    PULONGLONG m_pTarget;
    PULONGLONG m_pControl;
	ULONGLONG m_nTerms;

    void Reset()
    {
    }

    CResult(void)
    {
      
    }

    void SaveResult(QuAlgorithm *algo)
    {
		
		m_nCost = algo->m_QuantumCost;
		m_nBits = algo->m_nBits;
		m_nGates = algo->m_nGates;
		m_nTerms = algo->m_nTerms;
		cout << "gates" << algo->m_nGates << endl;
		cout << "bits" << algo->m_nBits << endl;
		cout << "cost" << algo->m_QuantumCost << endl;

		Console::WriteLine("----------------------------------------------------");
	  for(int i=0;i<algo->m_nTerms;i++)
		  Console::Write(algo->m_pIn[i]+",");
	  Console::WriteLine("\n----------------------------------------------------");
	  for(int i=0;i<algo->m_nTerms;i++)
		  Console::Write(algo->m_pOut[i]+",");
	  Console::WriteLine("\n----------------------------------------------------");

	  for(int i=0;i<algo->m_nGates;i++)
		  Console::Write(algo->m_pControl[i]+",");
	  Console::WriteLine("\n----------------------------------------------------");

	  for(int i=0;i<algo->m_nGates;i++)
		  Console::Write(algo->m_pTarget[i]+",");
	  Console::WriteLine("\n----------------------------------------------------");

	  m_pTarget = new ULONGLONG[m_nGates];
	   m_pControl = new ULONGLONG[m_nGates];
	   m_pIn = new ULONGLONG[m_nTerms];
	   m_pOut = new ULONGLONG[m_nTerms];

	   for(int j=0;j<m_nGates;j++)
	   {
		m_pTarget[j] = algo->m_pTarget[j];
		m_pControl[j] = algo->m_pControl[j];
	   }
	   for(int j=0;j<m_nTerms;j++)
	   {
		m_pIn[j] = algo->m_pIn[j];
		m_pOut[j] = algo->m_pOut[j];
	   }
        //COPY(m_pTarget, algo->m_pTarget, algo->m_nGates);
        //COPY(m_pControl, algo->m_pControl, algo->m_nGates);
		//COPY(m_pIn, algo->m_pIn, algo->m_nTerms);
        //COPY(m_pOut, algo->m_pOut, algo->m_nTerms);

        

    }

    void PrintResult(String ^fName, int iteration)
    {
	  String ^path = "..\\..\\SavedData\\";
	  Directory::CreateDirectory( path );
	  path += m_nBits.ToString() + "-bits\\";
	  Directory::CreateDirectory( path );
	  path += iteration.ToString() + "-iteration\\";
	  Directory::CreateDirectory( path );
	  StreamWriter ^file = gcnew StreamWriter(path + fName + ".qsy");
	  file->WriteLine("QuantomCost:");
	  file->WriteLine(System::Convert::ToString(m_nCost));
	  
	  file->WriteLine("Input Data:");
	  for(int i=0;i<Math::Pow(2,(double)m_nBits); i++)
		  file->Write(m_pIn[i]+",");
	  file->WriteLine();
	  file->WriteLine("Output Data:");
	  for(int i=0;i<Math::Pow(2,(double)m_nBits); i++)
		  file->Write(m_pOut[i]+",");
	  file->WriteLine();
	  
	  file->WriteLine("Control:");
	  for(int i=0;i<m_nGates;i++)
		  file->Write(m_pControl[i]+",");
	  file->WriteLine();
	  file->WriteLine("Target:");
	  for(int i=0;i<m_nGates;i++)
		  file->Write(m_pTarget[i]+",");
	  file->Close();
	  delete m_pTarget;
	  delete m_pControl;
	  delete m_pIn;
	  delete m_pOut;
    }
    
  };
}

ref class CResult
{
public:

	CResult(void)
	{
	}
};
