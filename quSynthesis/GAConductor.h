#pragma once
#define N_POP 100

#define CROSS_OVER SinglePointCrossOver
#define CROSS_OVER2 TwoPointCrossOver

#define RECYCLE(x) if (x) {delete x; x=NULL;}
#define COPY(dst, src, n) {if(dst) delete dst; dst = new int[n]; CopyMemory(dst, src, n);}

#include "CoreSynthesizer.h"

namespace Conductor
{
  class GeneticAlgorithm: public Conductor::Core, public GAParam {

  private:
    int m_BestFit;
    int m_nTerms;
    double m_ParentTotalFitness;

    FILE * pFile;
    Synthesizer::Ternary::Core *m_pSynthesizer;
    Algorithm::Ternary::Core m_pAlgo[N_POP];

  private:
    //    QuAlgorithm *m_pAlgo[N_POP*2];

  public:

    // <summary>
    // 
    // <inputs>
    //
    // <outputs>
    ~GeneticAlgorithm()
    {
      Release();
    }

    // <summary>
    // 
    // <inputs>
    //
    // <outputs>

    GeneticAlgorithm(Ternary::CoreSynthesizer *pSynthesizer) : Conductor::Core
    {
      m_pSynthesizer = pSynthesizer;
    }
    // <summary>
    // 
    // <inputs>
    //
    // <outputs>
    void InitPopulation()
    {
      // Setup population of individuals randomly
      for (int i=0; i<N_POP; i++) {
        m_pAlgo[i] = new Algorithm::Ternary::OrderedSet(m_pSynthesizer->m_nBits);
      }
    }

    void Release()
    {
      for (int i=0; i<N_POP; i++)
        if (m_pAlgo[i]) {
          delete m_pAlgo[i];
          m_pAlgo[i] = NULL;
        }
    }


    // <summary>
    // 
    // <inputs>
    //
    // <outputs>
    void Synthesize(PINT pOut)
    {
      InitPopulation();

      // Convert decimal representation to ternary (2-bit) format
      Ternary::Algorithm::Prepare(pOut);

      int iteration = 0;
      while(NextGAParam()) {
        CStopWatch s;
        s.startTimer();

        m_BestFit = MAXLONG;

        long counter = 0;
        // Calculate Cost Function for all individuals
        for (int r=0; r<m_nRun; r++) {
          Console::WriteLine("Run: {0}", r);
          for (int g=0; g<m_nGen; g++) {
            DoGeneration(g, pOut);
          }
        }
        s.stopTimer();
        PrintResult(++iteration, s.getElapsedTime());
      }

      Release();
    }

    // <summary>
    // 
    // <inputs>
    //
    // <outputs>
    void DoGeneration(int gen, PINT pOut)
    {
      m_ParentTotalFitness = 0;
      for (int i=0; i<N_POP; i++) {
        m_pSynthesizer->Add(m_pAlgo[i]);
      }

      m_pSynthesizer->Synthesize();

      for (int i=0; i<N_POP; i++) {
        int qCost = m_pAlgo[i]->m_QuantumCost;
        m_ParentTotalFitness += qCost;
        if ( (gen % 10) == 0)
          Console::Write("Gen: {0}, BestCost: {1}\r", gen, m_BestFit);

        if (m_BestFit > qCost) {
          m_BestFit = qCost;
          Console::WriteLine("Gen: {0}, BestCost: {1}", gen, m_BestFit);
          SaveResult(m_pAlgo[i]);
        }
      }

      Breed();
      Cull();
    }

    // <summary>
    // 
    // <inputs>
    //
    // <outputs>
    void Cull()
    {
      for (int i=0; i<N_POP; i++) {
        delete m_pAlgo[i];
        m_pAlgo[i] = m_pAlgo[i+N_POP];
      }
    }

    // <summary>
    // 
    // <inputs>
    //
    // <outputs>
    void Breed()
    {
      //for (int i=0; i<N_POP; i++) {
      //  QuAlgorithm *p1 = Roulette();  
      //  QuAlgorithm *p2 = Roulette();
      //  m_pAlgo[i+N_POP] = m_nCrossOver == 0 ? p1->SinglePointCrossOver(p2, m_Pc) : p1->TwoPointCrossOver(p2, m_Pc);
      //  m_pAlgo[i+N_POP]->Mutate(m_Pm);
      //}
    }

    // <summary>
    // 
    // <inputs>
    //
    // <outputs>
    QuAlgorithm *Roulette()
    {
      //double rnd = Rand::NextDouble();
      //double val=0;

      //for (int i=0; i < N_POP; i++) {
      //  val += m_pAlgo[i]->m_QuantumCost/m_ParentTotalFitness;
      //  if (rnd < val)
      //    return m_pAlgo[i];
      //}

      //return m_pAlgo[N_POP-1];
    }
  };
}

