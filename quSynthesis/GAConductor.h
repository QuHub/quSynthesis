#pragma once
#define N_POP 100

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

    double m_ParentTotalFitness;
    gcroot<Type^> m_AlgoType;

    FILE * pFile;

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
      m_nBits = nBits;
    }

    void InitPopulation()
    {
      // Setup population of individuals randomly
      for (int i=0; i<N_POP; i++) {
        // STEP(2): Add your new algorithm here...
        if(m_AlgoType->Name == "CoveredSetPartition") m_pAlgo[i] = new CoveredSetPartition(m_nBits);
        if(m_AlgoType->Name == "TernaryOrderedSet") m_pAlgo[i] = new TernaryOrderedSet(m_nBits);
      }
    }

   

    void Synthesize(PINT pOut)
    {
      InitPopulation();

      // Convert decimal representation to ternary (2-bit) format
      if(m_AlgoType->Name == "TernaryOrderedSet") TernaryOrderedSet::Prepare(pOut);

      int iteration = 0;
      while(NextGAParam()) {
        CStopWatch s;
        s.startTimer();
        
        m_BestFit = MAXLONG;
        //Directory::CreateDirectory( String::Format("..\\..\\SaveData\\{0}-bits", m_nBits));
        //file = gcnew StreamWriter(String::Format("..\\..\\SaveData\\{0}-bits\\costs{1}", m_nBits, iteration+1) + ".qsy");
        //file->Close();
        //delete file;

        long counter = 0;
        // Calculate Cost Function for all individuals
        for (int r=0; r<m_nRun; r++) {
          Console::WriteLine("Run: {0}", r);
          for (int g=0; g<m_nGen; g++) {
            DoGeneration(g, pOut);

            //counter++;
            //file = gcnew StreamWriter(String::Format("..\\..\\SaveData\\{0}-bits\\costs{1}", m_nBits, iteration+1) + ".qsy", true);
            //file->WriteLine(Convert::ToString(counter) +":" + Convert::ToString(m_BestFit));
            //file->Close();
            //delete file;
          }

        }
        s.stopTimer();
        PrintResult(++iteration, s.getElapsedTime());
      }
    }

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

