#pragma once
// BestCost         3261193(M=3)  3398744(M=2)      3407896       3398961
#define N_POP 100 // 100             100           200
#define N_GEN 300 // 200             300           500
#define N_RUN 10  // 10              10            20
#define PM    0.3 // 0.3             0.15          0.1
#define PC    0.7 // 0.7             0.6           0.6



#define CROSS_OVER SinglePointCrossOver
//#define CROSS_OVER TwoPointCrossOver

namespace QuLogic 
{
  class GAConductor: QuConductor {

  private:
    int m_BestFit;
    double m_ParentTotalFitness;
    gcroot<Type^> m_AlgoType;

  public:
    QuAlgorithm *m_pAlgo[N_POP*2];

    ~GAConductor()
    {
      for (int i=0; i<N_POP; i++)
        delete m_pAlgo[i];
    }

    GAConductor(int nBits, Type^ T) : QuConductor(nBits)
    {
      m_AlgoType = T;
      ZeroMemory(m_pAlgo, sizeof(PVOID) * 2 * N_POP);
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

