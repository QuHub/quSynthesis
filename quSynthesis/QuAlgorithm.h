#pragma once
namespace QuLogic {

  class QuAlgorithm 
  {
  public:
    ULONGLONG m_nBits;
    ULONGLONG m_nTerms;
    ULONGLONG m_nGates;
    ULONGLONG m_QuantumCost;
    PCHAR Name;
    PULONGLONG m_pIn;
    PULONGLONG m_pOut;
    PULONGLONG m_pTarget;
    PULONGLONG m_pControl;

  public:
    QuAlgorithm(void){}

    QuAlgorithm(int nBits, int nTerms)
    {
      m_nBits = nBits;
      m_nTerms = nTerms;
      m_pIn = m_pOut = NULL;
      m_pTarget = m_pControl = NULL;
    }

    // Copy constructor
    QuAlgorithm(const QuAlgorithm& base)
    {
      m_nBits = base.m_nBits;
      m_nTerms = base.m_nTerms;
      m_nGates = base.m_nGates;
      m_QuantumCost = base.m_QuantumCost;
      Name = base.Name;
      m_pOut = base.m_pOut;

      m_pIn = new ULONGLONG[m_nTerms];
      CopyMemory(m_pIn, base.m_pIn, m_nTerms*sizeof(ULONGLONG));
      m_pControl = m_pTarget = NULL;
    }

    void virtual Synthesize(PULONGLONG pOut) {throw "Must implement this";}
    void virtual Mutate(double Prob) {throw "Must implement this";}
    QuAlgorithm virtual *Copy() {throw "Must implement this";}
    QuAlgorithm virtual *Clone() {
      QuAlgorithm *p = this->Copy();
      p->m_pControl = new ULONGLONG[m_nGates];
      CopyMemory(p->m_pControl, this->m_pControl, m_nGates*sizeof(ULONGLONG));

      p->m_pTarget = new ULONGLONG[m_nGates];
      CopyMemory(p->m_pTarget, this->m_pTarget, m_nGates*sizeof(ULONGLONG));
      return p;
    }
    QuAlgorithm virtual *SinglePointCrossOver(QuAlgorithm *p, double Prob){throw "Must implement this";}
    QuAlgorithm virtual *TwoPointCrossOver(QuAlgorithm *p, double Prob){throw "Must implement this";}
    void inline Delete(PULONGLONG p) {if(p) delete p;}
    virtual ~QuAlgorithm(){
      Delete(m_pIn); 
      Delete(m_pControl); 
      Delete(m_pTarget);
    }
  };
}
