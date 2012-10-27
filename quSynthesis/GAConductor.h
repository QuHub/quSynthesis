#pragma once
#include "Function.h"

// BestCost         3261193(M=3)  3398744(M=2)      3407896       3398961
#define N_POP 100 // 100             100           200

#include <iostream>
using namespace System;
using namespace System::IO;
#define CROSS_OVER SinglePointCrossOver
#define CROSS_OVER2 TwoPointCrossOver

using namespace System::Runtime::InteropServices;

#define RECYCLE(x) if (x) {delete x; x=NULL;}
#define COPY(dst, src, n) {if(dst) delete dst; dst = new ULONGLONG[n]; CopyMemory(dst, src, n);}

namespace QuLogic 
{
  class GAConductor: public QuConductor {

  private:
    int m_BestFit;
    int m_nTerms;
	int m_nBits;
    int m_nGen;
    int m_nRun;
    double m_Pm;
    double m_Pc;
    int m_nCrossOver;
    gcroot<StreamReader^> m_sr;

    double m_ParentTotalFitness;
    gcroot<Type^> m_AlgoType;

	FILE * pFile;

  public:
    QuAlgorithm *m_pAlgo[N_POP*2];

    ~GAConductor()
    {
      for (int i=0; i<N_POP; i++)
        delete m_pAlgo[i];

      delete m_sr;
    }

    GAConductor(Function^ function, Type^ T) : QuConductor(function)
    {
      m_sr = gcnew StreamReader("..\\quSynthesis\\GAParams.csv");
      m_sr->ReadLine();  // Skip Header;

      m_AlgoType = T;
      ZeroMemory(m_pAlgo, sizeof(PVOID) * 2 * N_POP);
      m_nBits = function->nBits();
      m_nTerms = function->nTerms();
    }

    void InitPopulation()
    {
      // Setup population of individuals randomly
      for (int i=0; i<N_POP; i++) {
        // STEP(2): Add your new algorithm here...
        if(m_AlgoType->Name == "CoveredSetPartition") m_pAlgo[i] = new CoveredSetPartition(m_function);
      }
    }

    bool ReadGAParams()
    {
      String ^s;
      if(m_sr->Peek() >= 0)
        s = m_sr->ReadLine();
      else
        return false;

       IntPtr p = Marshal::StringToHGlobalAnsi(s);
       m_GAParams = static_cast<char*>(p.ToPointer());
        
       Marshal::FreeHGlobal(p);
      
      Console::WriteLine("Configuration: {0}", s);
      array<String^>^ list = s->Split(',');

      if (list->Length == 5) {
        // m_nGen,m_nRun,m_Pm,m_Pc
        m_nGen = Convert::ToUInt64(list[0]);
        m_nRun = Convert::ToUInt64(list[1]);
        m_Pm = Convert::ToDouble(list[2]);
        m_Pc = Convert::ToDouble(list[3]);
        m_nCrossOver = list[4] == "Single" ? 0 : 1;
        return true;
      }
      return false;
    }

    void Synthesize()
    {
      InitPopulation();
	  int iteration = 0;
	  StreamWriter ^file;
      while(ReadGAParams()) {
        m_BestFit = MAXLONGLONG;
		Directory::CreateDirectory( String::Format("..\\..\\SaveData\\{0}-bits", m_nBits));
		file = gcnew StreamWriter(String::Format("..\\..\\SaveData\\{0}-bits\\costs{1}", m_nBits, iteration+1) + ".qsy");
		file->Close();
		long counter = 0;
        // Calculate Cost Function for all individuals
        for (int r=0; r<m_nRun; r++) {
          Console::WriteLine("Run: {0}", r);
          for (int g=0; g<m_nGen; g++) {
            DoGeneration(g);

			counter++;
			file = gcnew StreamWriter(String::Format("..\\..\\SaveData\\{0}-bits\\costs{1}", m_nBits, iteration+1) + ".qsy", true);
			file->WriteLine(Convert::ToString(counter) +":" + Convert::ToString(m_BestFit));
			
			file->Close();
          }
          
        }
		PrintResult(++iteration);
		
      }
	  
    }

    void DoGeneration(int gen)
    {
      m_ParentTotalFitness = 0;
      for (int i=0; i<N_POP; i++) {
        m_pAlgo[i]->Synthesize();
      }

      WaitForQueue();

      for (int i=0; i<N_POP; i++) {
        ULONGLONG qCost = m_pAlgo[i]->m_QuantumCost;
        m_ParentTotalFitness += qCost;
        if (m_BestFit > qCost) {
          m_BestFit = qCost;
          Console::WriteLine("Gen: {0}, BestCost: {1}", gen, m_BestFit);
		  SaveResult(m_pAlgo[i]);
        }
      }

      Breed();
      Cull();
    }

    void Cull()
    {
      for (int i=0; i<N_POP; i++) {
        delete m_pAlgo[i];
        m_pAlgo[i] = m_pAlgo[i+N_POP];
      }
    }

    void Breed()
    {
      for (int i=0; i<N_POP; i++) {
        QuAlgorithm *p1 = Roulette();  
        QuAlgorithm *p2 = Roulette();
        m_pAlgo[i+N_POP] = m_nCrossOver == 0 ? p1->SinglePointCrossOver(p2, m_Pc) : p1->TwoPointCrossOver(p2, m_Pc);
        m_pAlgo[i+N_POP]->Mutate(m_Pm);
      }
    }


    QuAlgorithm *Roulette()
    {
      double rnd = Rand::NextDouble();
      double val=0;

      for (int i=0; i < N_POP; i++) {
        val += m_pAlgo[i]->m_QuantumCost/m_ParentTotalFitness;
        if (rnd < val)
          return m_pAlgo[i];
      }

      return m_pAlgo[N_POP-1];
    }
  };
}

