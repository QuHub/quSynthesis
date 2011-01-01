#pragma once

#define N_POP 100
#define N_GEN 400
#define N_RUN 10
#define PM 0.45
#define PC 0.6
#include <vcclr.h>
using namespace System::Reflection;

namespace QuLogic 
{
  class GAConductor: QuConductor {

  private:
    int m_bestFit;
    int m_m_parentFitness;
    gcroot<Random^> m_rnd; 

  public:
    QuAlgorithm *m_pAlgo[N_POP*2];

    ~GAConductor()
    {
      for (int i=0; i<N_POP; i++)
        delete m_pAlgo[i];
    }

    GAConductor(int nBits, Type^ T) : QuConductor()
    {
      m_rnd = gcnew Random();
      // Setup population of individuals randomly
      for (int i=0; i<N_POP; i++) {
        // STEP(2): Add your new algorithm here...
        if(T->Name == "CoveredSetPartition") m_pAlgo[i] = new CoveredSetPartition(nBits);
      }
    }

    void Synthesize(PULONGLONG pOut)
    {
      // Calculate Cost Function for all individuals
      for (int r=0; r<N_RUN; r++) {
        for (int g=0; g<N_GEN; g++) {
          DoGeneration(g);
        }
      }
    }

    void DoGeneration(int gen)
    {
      //m_parentFitness = 0;
      //m_bestFit = MAXLONGLONG;
      //for (int i=0; i<N_POP; i++) {
      //  m_pAlgo[i]->Synthesize(pOut);

      //  parentFitness += m_pAlgo[i]->Cost();
      //  if (bestFit > m_pAlgo[i]->Cost())
      //    bestFit = m_pAlgo[i]->Cost();
      //}

      //Breed();
      //Cull();
    }

    void Breed()
    {
      //for (int i=0; i<N_POP; i++) {
      //  QuAlgorithm *p1 = Roulette();  
      //  QuAlgorithm *p1 = Roulette();
      //  m_pAlgo[i+N_POP] = CROSS_OVER(p1, p2, PC);
      //  m_pAlgo[i+N_POP]->Mutate();
      //}
    }

    QuAlgorithm *Roulette()
    {
      //double rnd = m_rnd.NextDouble();
      //double val=0;

      //for (int i=0; i < POP_SIZE; i++) {
      //  val += m_pAlgo[i]->QauntumCost()/m_ParentTotalFitness;
      //  if (rnd < val)
      //    return m_pAlgo[i];
      //}

      return m_pAlgo[N_POP-1];
    }
  };


}

