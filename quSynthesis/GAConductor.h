#pragma once
#define N_POP 512 

#define CROSS_OVER SinglePointCrossOver
#define CROSS_OVER2 TwoPointCrossOver

#define RECYCLE(x) if (x) {delete x; x=NULL;}
#define COPY(dst, src, n) {if(dst) delete dst; dst = new int[n]; CopyMemory(dst, src, n);}

namespace QuLogic 
{
  class GAConductor: public QuConductor, public GAParam {

  private:
    int m_BestFit;
    int m_nTerms;
    int m_RoletteWheel[1000];

    double m_ParentTotalFitness;

    FILE * pFile;

  private:
    QuAlgorithm *m_pAlgo[N_POP*2];

  public:

    // <summary>
    // 
    // <inputs>
    //
    // <outputs>
    ~GAConductor()
    {
      Release();
    }

    // <summary>
    // 
    // <inputs>
    //
    // <outputs>
    GAConductor(int nBits, Type^ T) : QuConductor(nBits)
    {
      m_AlgoType = T;
      m_nBits = nBits;
      ZeroMemory(m_pAlgo, sizeof(PVOID) * 2 * N_POP);
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
        // STEP(2): Add your new algorithm here...
        if(m_AlgoType->Name == "CoveredSetPartition") m_pAlgo[i] = new CoveredSetPartition(m_nBits);
        if(m_AlgoType->Name == "OrderedSet") m_pAlgo[i] = new Ternary::Algorithm::OrderedSet(m_nBits);
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
      if(m_AlgoType->Name == "OrderedSet") Ternary::Algorithm::Prepare(pOut);

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
        PrintResult(++iteration, s.getElapsedTime(), ParametersForDisplay(), FunctionName);
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
        m_pAlgo[i]->Synthesize(pOut);
      }

      WaitForQueue();

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
      InitializeRoletteWheel();
      for (int i=0; i<N_POP; i++) {
        QuAlgorithm *p1 = Roulette();  
        QuAlgorithm *p2 = Roulette();
        m_pAlgo[i+N_POP] = m_nCrossOver == 0 ? p1->SinglePointCrossOver(p2, m_Pc) : p1->TwoPointCrossOver(p2, m_Pc);
        m_pAlgo[i+N_POP]->Mutate(m_Pm);
      }
    }

#define ROLETTE_SIZE (sizeof(m_RoletteWheel)/sizeof(m_RoletteWheel[0]))
    void InitializeRoletteWheel()
    {
      double scale = (double)ROLETTE_SIZE/(double)m_ParentTotalFitness;
      int total = 0;
      int index = 0;
      for(int i=0; i<N_POP; i++) {
        total += m_pAlgo[i]->m_QuantumCost;
        int last = Math::Round(total * scale);
        while(index < last && index < ROLETTE_SIZE)
          m_RoletteWheel[index++] = i;

        index = last;
      }
    }

    // <summary>
    // 
    // <inputs>
    //
    // <outputs>
    QuAlgorithm *Roulette()
    {
      int rnd = Rand::NextInteger(ROLETTE_SIZE - 1);
      return m_pAlgo[m_RoletteWheel[rnd]];
    }
  };
}

