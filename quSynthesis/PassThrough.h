#pragma once

#define RECYCLE(x) if (x) {delete x; x=NULL;}
#define COPY(dst, src, n) {if(dst) delete dst; dst = new int[n]; CopyMemory(dst, src, n);}

namespace QuLogic 
{
  class PassThrough: public QuConductor {

  private:
    int m_BestFit;
    int m_nTerms;

    FILE * pFile;

  private:
    QuAlgorithm *m_pAlgo[1];

  public:

    // <summary>
    // 
    // <inputs>
    //
    // <outputs>
    ~PassThrough()
    {
      delete m_pAlgo[0];
    }

    // <summary>
    // 
    // <inputs>
    //
    // <outputs>
    PassThrough(int nBits, Type^ T) : QuConductor(nBits)
    {
      m_AlgoType = T;
      m_nBits = nBits;
      if(m_AlgoType->Name == "NaturalOrder") m_pAlgo[0] = new Ternary::Algorithm::NaturalOrder(m_nBits);
    }


    // <summary>
    // 
    // <inputs>
    //
    // <outputs>
    void Synthesize(PINT pOut)
    {
		  CStopWatch s;
		  s.startTimer();

      m_pAlgo[0]->Synthesize(pOut);
      s.stopTimer();

      WaitForQueue();
      Console::Write("BestCost: {0}\n", m_pAlgo[0]->m_QuantumCost);
      SaveResult(m_pAlgo[0]);
	    PrintResult(0, s.getElapsedTime(), "Standalone Synthesis", FunctionName);
    }
  };
}
