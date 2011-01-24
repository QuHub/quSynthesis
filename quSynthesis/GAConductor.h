#pragma once
// BestCost         3261193(M=3)  3398744(M=2)      3407896       3398961
#define N_POP 100 // 100             100           200
//#define N_GEN 300 // 200             300           500
//#define N_RUN 10  // 10              10            20
//#define PM    0.3 // 0.3             0.15          0.1
//#define PC    0.7 // 0.7             0.6           0.6

// N_POP N_GEN N_RUN PM PC
//  100  300  10 0.3. 0.7


#include <iostream>
using namespace System;
using namespace System::IO;
#define CROSS_OVER SinglePointCrossOver
#define CROSS_OVER2 TwoPointCrossOver

#define RECYCLE(x) if (x) {delete x; x=NULL;}
#define COPY(dst, src, n) {if(dst) delete dst; dst = new ULONGLONG[n]; CopyMemory(dst, src, n);}

namespace QuLogic 
{
  class GAConductor: public QuConductor {

  private:
    int m_BestFit;
	int nBestFit;
	int m_nTerms;

	//int N_POP;
	int N_GEN;
    int N_RUN;
	double PM;
	double PC;


    double m_ParentTotalFitness;
    gcroot<Type^> m_AlgoType;

	

  public:
    QuAlgorithm *m_pAlgo[N_POP*2];
	  //QuAlgorithm *m_pAlgo;
	
    ~GAConductor()
    {
      for (int i=0; i<N_POP; i++)
        delete m_pAlgo[i];
    }

    GAConductor(int nBits, Type^ T, int n_gen, int n_run, double pm, double pc) : QuConductor(nBits)
    {
	  // Read a file that has the parameters
		//N_POP = 100 ;
		N_GEN = n_gen ;
		N_RUN = n_run  ;
		PM    = pm ;
		PC    = pc ;
		
      m_AlgoType = T;
      ZeroMemory(m_pAlgo, sizeof(PVOID) * 2 * N_POP);
	  nBestFit = 0;
    }

    void InitPopulation()
    {
      // Setup population of individuals randomly
      for (int i=0; i<N_POP; i++) {
        // STEP(2): Add your new algorithm here...
        if(m_AlgoType->Name == "CoveredSetPartition") m_pAlgo[i] = new CoveredSetPartition(m_nBits);
      }
    }

    void Synthesize(PULONGLONG pOut)
    {
      m_BestFit = MAXLONGLONG;
	  
      // Calculate Cost Function for all individuals
      for (int r=0; r<N_RUN; r++) {
        Console::WriteLine("Run: {0}", r);
        InitPopulation();
        for (int g=0; g<N_GEN; g++) {
          DoGeneration(g, pOut);
        }
      }

	  m_pAlgo[nBestFit]->m_QuantumCost = m_BestFit;
	  m_pAlgo[nBestFit]->m_nGates = m_nGates;

	  
	  m_pAlgo[nBestFit]->m_pControl = new ULONGLONG[m_nGates];
	  m_pAlgo[nBestFit]->m_pTarget = new ULONGLONG[m_nGates];
	  m_pAlgo[nBestFit]->m_pIn = new ULONGLONG[m_nTerms];
	  m_pAlgo[nBestFit]->m_pOut = new ULONGLONG[m_nTerms];
	  
	  for(int j=0;j<m_nGates;j++)
	  {
		  m_pAlgo[nBestFit]->m_pControl[j] = m_pControl[j];
		  m_pAlgo[nBestFit]->m_pTarget[j] = m_pTarget[j];
	  }
	  for(int j=0;j<m_nTerms;j++)
	  {
		  m_pAlgo[nBestFit]->m_pIn[j] = m_pIn[j];
		  m_pAlgo[nBestFit]->m_pOut[j] = m_pOut[j];
	  }
	  
	  delete m_pControl;
	  delete m_pTarget;
	  delete m_pIn;
	  delete m_pOut;

	  SaveResult(m_pAlgo[nBestFit]);
    }

    void DoGeneration(int gen, PULONGLONG pOut)
    {
      m_ParentTotalFitness = 0;
      for (int i=0; i<N_POP; i++) {
        m_pAlgo[i]->Synthesize(pOut);
      }

      WaitForQueue();

      for (int i=0; i<N_POP; i++) {
        ULONGLONG qCost = m_pAlgo[i]->m_QuantumCost;
        m_ParentTotalFitness += qCost;
        if (m_BestFit > qCost) {
          m_BestFit = qCost;
		  Console::WriteLine("Gen: {0}, BestCost: {1}", gen, m_BestFit);
		  m_nGates = m_pAlgo[i]->m_nGates;
		  m_nTerms = m_pAlgo[i]->m_nTerms;

		  m_pControl = new ULONGLONG[m_nGates];
		  m_pTarget = new ULONGLONG[m_nGates];
		  for(int j=0;j<m_nGates;j++)
		  {
			  m_pTarget[j] = m_pAlgo[i]->m_pTarget[j];
			  m_pControl[j] = m_pAlgo[i]->m_pControl[j];
		  }

		  m_pIn = new ULONGLONG[m_nTerms];
		  m_pOut = new ULONGLONG[m_nTerms];

		  for(int j=0;j<m_nTerms;j++)
		  {
			  m_pIn[j] = m_pAlgo[i]->m_pIn[j];
			  m_pOut[j] = m_pAlgo[i]->m_pOut[j];
		  }
		  /*
		  Console::WriteLine("Input:");
		  for(int j=0;j<m_pAlgo[i]->m_nTerms;j++)
			  Console::Write(m_pAlgo[i]->m_pIn[j] + ",");
		  Console::WriteLine("\n-------------------------------------------------");
		  Console::WriteLine("Output:");
		  for(int j=0;j<m_pAlgo[i]->m_nTerms;j++)
			  Console::Write(m_pAlgo[i]->m_pOut[j] + ",");
		  Console::WriteLine("\n-------------------------------------------------");
		  Console::WriteLine("Control:");
		  for(int j=0;j<m_pAlgo[i]->m_nGates;j++)
			  Console::Write(m_pAlgo[i]->m_pControl[j] + ",");
		  Console::WriteLine("\n-------------------------------------------------");
		  Console::WriteLine("Target:");
		  for(int j=0;j<m_pAlgo[i]->m_nGates;j++)
			  Console::Write(m_pAlgo[i]->m_pTarget[j] + ",");
		  Console::WriteLine("\n-------------------------------------------------");
		  // till here everyting is ok with control & target
		  */
		  nBestFit = i;
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
        m_pAlgo[i+N_POP] = p1->CROSS_OVER(p2, PC);
		//m_pAlgo[i+N_POP] = p1->CROSS_OVER(p2, PC);

        m_pAlgo[i+N_POP]->Mutate(PM);
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

