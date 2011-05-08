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
    PULONGLONG m_pOperation;
    gcroot<Type^> m_SynthesisAlgo;
    
  public:
    QuAlgorithm(void){}

    QuAlgorithm(int nBits)
    {
      m_nBits = nBits;
      m_nTerms = (ULONGLONG)pow(QuLogic::Radix,(double)nBits);
      m_pIn = m_pOut = NULL;
      m_nGates = 0;
      m_pTarget = m_pControl = m_pOperation = NULL;
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
      m_pTarget = m_pControl = m_pOperation = NULL;
    }

    void Dump()
    {
      cout << "In   |    Out" << endl;
      for (int i=0; i<m_nTerms; i++)
        cout << m_pIn[i] << " | " << m_pOut[i] << endl;

      cout << "Target | Control | Operation" << endl;

      for (int i=0; i<m_nGates; i++)
        cout << m_pTarget[i] << " | " << m_pControl[i] << " | " << m_pOperation[i] << endl;
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

      p->m_pOperation= new ULONGLONG[m_nGates];
      CopyMemory(p->m_pOperation, this->m_pOperation, m_nGates*sizeof(ULONGLONG));

      return p;
    }
    QuAlgorithm virtual *SinglePointCrossOver(QuAlgorithm *p, double Prob){throw "Must implement this";}
    QuAlgorithm virtual *TwoPointCrossOver(QuAlgorithm *p, double Prob){throw "Must implement this";}
    void inline Delete(PULONGLONG p) {if(p) delete p;}
    virtual ~QuAlgorithm(){
      Delete(m_pIn); 
      Delete(m_pControl); 
      Delete(m_pTarget);
      Delete(m_pOperation);
    }
  };
}
